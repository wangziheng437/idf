/*
 * @Description: 
 * @Author: 
 * @Date: 2019-06-14 16:37:30
 * @LastEditTime: 2019-07-09 00:14:14
 */

#include <netdb.h>
#include <sys/socket.h>

#include "esp_system.h"
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/esp_debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"

#include "sys_base.h"
#include "sys_https.h"

/**
 * @description:  
 */
sys_https_info_cb_fun sys_https_info_cb;
sys_https_info_t https_info = {};

/**
 * @description:  
 */
void sys_https_info_execute_cb(sys_https_info_t *info);
int sys_https_post(char *url ,char *host ,char *port, char *type, char *msg, char *out, int out_len);
int sys_https_get(char *url ,char *host ,char *port, char *msg, char *out, int out_len);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_https_info_sign_cb(sys_https_info_cb_fun fun)
{
	sys_https_info_cb = fun;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_https_info_execute_cb(sys_https_info_t *info)
{	
    if(sys_https_info_cb == NULL)
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        return;
    }
	sys_https_info_cb(info);
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_https_mbedtls_init(mbedtls_ssl_context *ssl, mbedtls_ctr_drbg_context *ctr,
		mbedtls_ssl_config *conf, mbedtls_entropy_context *ent, char *host)
{
	int rc = 0;

    https_info.step = SYS_HTTPS_CB_STEP_INIT;
    if((ssl == NULL) || (ctr == NULL) || (conf == NULL) || (ent == NULL) || (host == NULL))
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        https_info.code = SYS_HTTPS_CODE_PARAM_NULL;
        sys_https_info_execute_cb(&https_info);
        return;
    }
    
	mbedtls_ssl_init(ssl);
	mbedtls_ctr_drbg_init(ctr);
	mbedtls_ssl_config_init(conf);
	mbedtls_entropy_init(ent);

	if(0 != (rc = mbedtls_ctr_drbg_seed(ctr, mbedtls_entropy_func, ent, NULL, 0)))
	{
		sys_log_error("%s:mbedtls_ctr_drbg_seed error! %d", SYS_GET_FUN_NAME, rc);
        https_info.code = rc;
        sys_https_info_execute_cb(&https_info);
		return;
	}

	if(0 != (rc = mbedtls_ssl_set_hostname(ssl, host)))
	{
        sys_log_error("%s:mbedtls_ssl_set_hostname error! -0x%x", SYS_GET_FUN_NAME, -rc);
        https_info.code = rc;
        sys_https_info_execute_cb(&https_info);
		return;
	}

	if(0 != (rc = mbedtls_ssl_config_defaults(conf,
										  MBEDTLS_SSL_IS_CLIENT,
										  MBEDTLS_SSL_TRANSPORT_STREAM,
										  MBEDTLS_SSL_PRESET_DEFAULT)))
	{
        sys_log_error("%s:mbedtls_ssl_config_defaults error! %d", SYS_GET_FUN_NAME, rc);
        https_info.code = rc;
        sys_https_info_execute_cb(&https_info);
		return;
	}
	mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
	mbedtls_ssl_conf_rng(conf, mbedtls_ctr_drbg_random, ctr);

	if (0 != (rc = mbedtls_ssl_setup(ssl, conf)))
	{
        sys_log_error("%s:mbedtls_ssl_setup error! -0x%x", SYS_GET_FUN_NAME, -rc);
        https_info.code = rc;
        sys_https_info_execute_cb(&https_info);
		return;
	}
    
	https_info.code = SYS_HTTPS_CODE_SUCCESS;
    sys_https_info_execute_cb(&https_info);
    return;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_https_mbedtls_connect(mbedtls_ssl_context *ssl, mbedtls_net_context *net, char *host, char *port)
{
	int rc = 0;

    https_info.step = SYS_HTTPS_CB_STEP_CON;
    if((ssl == NULL) || (net == NULL) || (host == NULL) || (port == NULL))
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        https_info.code = SYS_HTTPS_CODE_PARAM_NULL;
        sys_https_info_execute_cb(&https_info);
        return;
    }

    sys_log_info("connecting:%s:%s...", host, port);
    
    mbedtls_net_init(net);
	if (0 != (rc = mbedtls_net_connect(net, host, port, MBEDTLS_NET_PROTO_TCP)))
	{
        sys_log_error("%s:mbedtls_net_connect error! -0x%x", SYS_GET_FUN_NAME, -rc);
        https_info.code = rc;
        sys_https_info_execute_cb(&https_info);
		return;
	}

    sys_log_debug("connected.");

	mbedtls_ssl_set_bio(ssl, net, mbedtls_net_send, mbedtls_net_recv, mbedtls_net_recv_timeout);

    sys_log_debug("Performing the SSL/TLS handshake...");
	
    #if(SYS_CORE_MODEL == SYS_ESP8266)
    rtc_clk_cpu_freq_set(RTC_CPU_FREQ_160M); //Increase the main frequency
    #endif

	while (0 != (rc = mbedtls_ssl_handshake(ssl)))
	{
		if (rc != MBEDTLS_ERR_SSL_WANT_READ && rc != MBEDTLS_ERR_SSL_WANT_WRITE )
		{
            #if(SYS_CORE_MODEL == SYS_ESP8266)
			rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
            #endif
            
            sys_log_error("%s:mbedtls_ssl_handshake error! -0x%x", SYS_GET_FUN_NAME, -rc);
            https_info.code = rc;
            sys_https_info_execute_cb(&https_info);
			return;
		}
	}
    
    #if(SYS_CORE_MODEL == SYS_ESP8266)
	rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);//Recovery of main frequency
    #endif

    sys_log_debug("cipher suite is %s", mbedtls_ssl_get_ciphersuite(ssl));

    https_info.code = SYS_HTTPS_CODE_SUCCESS;
    sys_https_info_execute_cb(&https_info);
	return;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_https_mbedtls_write(mbedtls_ssl_context *ssl, char *buf, int buf_len)
{
	int rc = 0;

    https_info.step = SYS_HTTPS_CB_STEP_WRITE;
    if((ssl == NULL) || (buf == NULL))
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        https_info.code = SYS_HTTPS_CODE_PARAM_NULL;
        sys_https_info_execute_cb(&https_info);
        return;
    }

	rc = mbedtls_ssl_write(ssl, (const unsigned char *)buf, buf_len);
	if (rc >= 0)
	{
        sys_log_debug("%s:%s", SYS_GET_FUN_NAME, buf);
        https_info.code = rc;
        sys_https_info_execute_cb(&https_info);	
		return;
	}
	else
	{
		if (rc != MBEDTLS_ERR_SSL_WANT_WRITE && rc != MBEDTLS_ERR_SSL_WANT_READ)
		{
            sys_log_error("%s:mbedtls_ssl_write error! -0x%x", SYS_GET_FUN_NAME, -rc);
            https_info.code = rc;
            sys_https_info_execute_cb(&https_info);
		}
        sys_log_error("%s:mbedtls_ssl_write fail!", SYS_GET_FUN_NAME);
		return;
	}
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_https_mbedtls_read(mbedtls_ssl_context *ssl, mbedtls_ssl_config *conf, char *out, int out_len, uint32_t out_time)
{
	int rc = 0;

    https_info.step = SYS_HTTPS_CB_STEP_READ;
    if((ssl == NULL) || (conf == NULL) || (out == NULL) || (out_len <= 0))
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        https_info.code = SYS_HTTPS_CODE_PARAM_NULL;
        sys_https_info_execute_cb(&https_info);
        return;
    }

	mbedtls_ssl_conf_read_timeout(conf, out_time);
	rc = mbedtls_ssl_read(ssl, (unsigned char *)out, out_len);

	if(rc == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
	{
		rc = 0;
	}
	if(rc == MBEDTLS_ERR_SSL_TIMEOUT)
	{
        sys_log_info("Read over");
		rc = 0;
	}
	if(rc < 0)
	{
        sys_log_error("%s:mbedtls_ssl_read error! -0x%x", SYS_GET_FUN_NAME, -rc);
        https_info.code = rc;
        sys_https_info_execute_cb(&https_info);
		return;
	}
	if(rc == 0)
	{
        sys_log_info("connection closed");
	}
	if(rc > 0)
	{
        sys_log_debug("%s:%d bytes read", SYS_GET_FUN_NAME, rc);
	}
    
    https_info.code = rc;
    sys_https_info_execute_cb(&https_info);
	return;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_https_mbedtls_again(mbedtls_ssl_context *ssl, mbedtls_net_context *net)
{
    https_info.step = SYS_HTTPS_CB_STEP_AGAIN;
    if((ssl == NULL) || (net == NULL))
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        https_info.code = SYS_HTTPS_CODE_PARAM_NULL;
        sys_https_info_execute_cb(&https_info);
        return;
    }

	mbedtls_ssl_close_notify(ssl);
	mbedtls_ssl_session_reset(ssl);
	mbedtls_net_free(net);
    https_info.code = SYS_HTTPS_CODE_SUCCESS;
    sys_https_info_execute_cb(&https_info);
    return;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_https_mbedtls_exit(mbedtls_ssl_context *ssl, mbedtls_ctr_drbg_context *ctr,
                             mbedtls_ssl_config *conf, mbedtls_entropy_context *ent)
{
    https_info.step = SYS_HTTPS_CB_STEP_EXIT;
    if((ssl == NULL) || (ctr == NULL) || (conf == NULL) || (ent == NULL))
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        https_info.code = SYS_HTTPS_CODE_PARAM_NULL;
        sys_https_info_execute_cb(&https_info);
        return;
    }
    
    mbedtls_ssl_config_free(conf);   //释放内存
	mbedtls_ctr_drbg_free(ctr);
	mbedtls_entropy_free(ent);
	mbedtls_ssl_free(ssl);
    https_info.code = SYS_HTTPS_CODE_SUCCESS;
    sys_https_info_execute_cb(&https_info);
    return;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
// int sys_https_rest(char *host, char *port, char *msg, int timeout)
// {
//     if((host == NULL) || (port == NULL) || (msg == NULL) || (timeout <= 0))
//     {
//         sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
//         return SYS_HTTPS_CODE_PARAM_NULL;  
//     }
    
// 	mbedtls_entropy_context ent;
// 	mbedtls_ctr_drbg_context ctr;
// 	mbedtls_ssl_context ssl;
// 	mbedtls_ssl_config conf;
// 	mbedtls_net_context net;


//     if(sys_https_mbedtls_init(&ssl,&ctr,&conf,&ent,host))
//     {
//         sys_https_mbedtls_again(&ssl,&net);
        
//     }

//     while((rc = sys_https_mbedtls_net(&ssl,&net,host,port))!=0)
//     {
//         sys_https_mbedtls_again(&ssl,&net);
        
//     }

// 	sys_https_mbedtls_write(&ssl,msg);
			
				

				
// 		int read_ret=sys_https_mbedtls_read(&ssl,&conf,buf,BUF_LEN,timeout);//超时太短在弱网时失败概率大
				
// 		sys_https_mbedtls_again(&ssl,&net);
		
// 		sys_https_mbedtls_exit(&ssl,&ctr,&conf,&ent);
		
   
	
// }