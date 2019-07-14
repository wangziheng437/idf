/*
 * @Description:
 * @Author: 
 * @Date: 2019-06-13 14:43:13
 * @LastEditTime: 2019-07-08 03:33:46
 */

#include "sys_wifi.h"
#include "sys_flash.h"
#include "sys_iot.h"
#include "sys_rtos.h"
#include "pro_iot.h"
#include "fun_base.h"
#include "fun_data.h"
#include "fun_iot.h"


/**
 * @description: 
 */


char *client_id = "94513d026e8b";


/**
 * @description: 
 */
void fun_iot_init_connect(void);
void fun_iot_reconnect(void);
void fun_iot_state_handle(void);
void fun_iot_info_callback(sys_iot_info_t *info);


/**
 * @description: 
 * @param: void
 * @return: 
 */
void fun_iot_init_connect(void)
{
	sys_flash_iot_crt_t *crt = NULL;

	sys_wifi_wait_connect();
    sys_flash_get_iot_crt(&crt);
	sys_iot_init(IOT_HOST_ADDR, IOT_PORT, crt->root, crt->crt, crt->key);
	sys_iot_connect(300, client_id);

}

/**
 * @description: 
 * @param: void
 * @return: 
 */
void fun_iot_reconnect(void)
{
	sys_flash_iot_crt_t *crt = NULL;
	
	sys_wifi_wait_connect();
    sys_flash_get_iot_crt(&crt);
	sys_iot_reinit(crt->root, crt->crt, crt->key);
	sys_iot_connect(300, client_id);
	sys_iot_resubscribe();
}

/**
 * @description: 
 * @param: void
 * @return: 
 */
void fun_iot_state_handle(void)
{
	char state = fun_data_get_iot_state();
	
	switch(state)
	{
		case FUN_IOT_STATE_CERT:
			sys_iot_disconnect();
			//下载证书
			fun_iot_reconnect();
			break;
		case FUN_IOT_STATE_UPGRADE:
			sys_iot_disconnect();
			//升级
			fun_iot_reconnect();
			break;
		case FUN_IOT_STATE_DELETE:
			//删除
			break;
		case FUN_IOT_STATE_WORK:
			sys_iot_yield(100);//心跳周期300s,实际60s一发，设置为0.1s一问
			sys_rtos_set_delay(1000);
			break;
		default:
			break;	
	}
}

/**
 * @description: 
 * @param: 
 * @return: 
 */
void fun_iot_info_callback(sys_iot_info_t *info)
{
	if(info == NULL)
	{
		fun_log_error("%s:param NULL!", FUN_GET_FUN_NAME);
		return;
	}

	switch (info->step)
	{
		case SYS_IOT_CB_STEP_INIT:
			if(info->code == SYS_IOT_CODE_SUCCESS)
			{
				fun_data_set_iot_state(FUN_IOT_STATE_INIT);
			}
			else
			{
				fun_log_error("%s:iot init fail! %d", FUN_GET_FUN_NAME, info->code);
			}
			break;
		case SYS_IOT_CB_STEP_CON:
			if(info->code == SYS_IOT_CODE_SUCCESS)
			{
				sys_flash_free_iot_crt();
				fun_data_set_iot_state(FUN_IOT_STATE_WORK);
			}
			else 
			{
				fun_log_error("%s:iot connect fail! %d", FUN_GET_FUN_NAME, info->code);
				if((info->code == SYS_IOT_ROOT_CRT_ERROR) || (info->code == SYS_IOT_DEVICE_CRT_ERROR)
					|| (info->code == SYS_IOT_PRIVATE_KEY_ERROR))  
				{
					sys_flash_free_iot_crt();
					fun_data_set_iot_state(FUN_IOT_STATE_CERT);//download certificate
				}
				else
				{
					sys_iot_connect(300, client_id);
				}
			}
			break;
		case SYS_IOT_CB_STEP_RECON:
			if(info->code == SYS_IOT_MANUAL_RECONNECT)
			{
				fun_log_error("iot reconnect start!");
				fun_iot_reconnect();
			}
			break;
		case SYS_IOT_CB_STEP_SUB:
			if(info->code == SYS_IOT_CODE_SUCCESS)
			{
				fun_log_info("sub ok");
			}
			else
			{
				fun_log_error("%s:subscribe fail! %d", FUN_GET_FUN_NAME, info->code);
			}
			break;
		case SYS_IOT_CB_STEP_RESUB:
			if(info->code == SYS_IOT_CODE_SUCCESS)
			{
				fun_log_info("resub ok");
			}
			else
			{
				sys_iot_resubscribe();
				fun_log_error("%s:resubscribe fail! %d", FUN_GET_FUN_NAME, info->code);
			}
			break;
		case SYS_IOT_CB_STEP_SUBCB1:
			if((info->sub_data1 != NULL) && (info->sub_data_len1 < 500))
			{
				fun_log_info("sub data:%s",info->sub_data1);
				fun_data_parse_iot_protocol(info->sub_data1);
			}
			else
			{
				fun_log_error("%s:subscribe data abnormal! %d", FUN_GET_FUN_NAME, info->code);
			}
			break;
		case SYS_IOT_CB_STEP_PUB:
			if(info->code == SYS_IOT_CODE_SUCCESS)
			{
				fun_log_info("pub ok");
			}
			else if(info->code == SYS_IOT_REQUEST_TIMEOUT)
			{
				fun_log_info("pub timeout!");
			}
			else
			{
				fun_log_error("%s:publish fail! %d", FUN_GET_FUN_NAME, info->code);
			}
			break;
		default:
			fun_log_error("%s:no step found %d!", FUN_GET_FUN_NAME, info->step);
			break;
	}
}

/**
 * @description: 
 * @param: void
 * @return: 
 */
void fun_iot_work_function(void)
{
	sys_iot_info_sign_cb(fun_iot_info_callback);
	
	fun_iot_init_connect();
	fun_iot_state_handle();
	
	char sub_topic1[50] = {};
	pro_iot_topic_choice(0,client_id, sub_topic1, sizeof(sub_topic1));
	sys_iot_subscribe(sub_topic1, strlen(sub_topic1), 1);
	
	char *payload = "one news ok";
	sys_iot_publish(sub_topic1, strlen(sub_topic1), payload, strlen(payload));
	
	while(1)
	{
		fun_iot_state_handle();
	}
}
