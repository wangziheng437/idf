/*
 * @Description: protocol of iot receive
 * @Author: 
 * @Date: 2019-06-17 18:26:05
 * @LastEditTime: 2019-07-08 23:39:50
 */

#include "pro_base.h"

/**
 * @description: 
 */


/**
 * @description: 
 */

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int pro_https_post(char *url ,char *host ,char *port, char *type, char *msg, char *out, int out_len)
{
    if((url == NULL) || (host == NULL) || (port == NULL) || (type == NULL) || (msg == NULL) || (out == NULL) || (out_len <= 0))
    {
        pro_log_error("%s:param NULL!", PRO_GET_FUN_NAME);
        return -1;  
    }

    int len = 0;
    len += snprintf(out + len, out_len - len, "POST %s HTTP/1.1\r\n", url);			
    len += snprintf(out + len, out_len - len, "HOST: %s:%s\r\n", host, port);
    len += snprintf(out + len, out_len - len, "Content-Type: %s\r\n", type);
    len += snprintf(out + len, out_len - len, "Content-Length: %d\r\n\r\n", strlen(msg));
    len += snprintf(out + len, out_len - len, "%s\r\n", msg);
    if(len < out_len)
	{
		pro_log_debug("%s:post = %s %d", PRO_GET_FUN_NAME, out, len);
		return len;
	}
    pro_log_error("%s:insufficient string length!", PRO_GET_FUN_NAME);
	return -2;
}


/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int pro_https_get(char *url ,char *host ,char *port, char *msg, char *out, int out_len)
{
    if((url == NULL) || (host == NULL) || (port == NULL) || (msg == NULL) || (out == NULL) || (out_len <= 0))
    {
        pro_log_error("%s:param NULL!", PRO_GET_FUN_NAME);
        return -1;  
    }

    int len = 0;
    len += snprintf(out + len, out_len - len, "GET %s HTTP/1.1\r\n", url);			
    len += snprintf(out + len, out_len - len, "HOST: %s:%s\r\n", host, port);
    len += snprintf(out + len, out_len - len, "Range: bytes=%d-\r\n", atoi(msg));  
    if(len < out_len)
	{
		pro_log_debug("%s:get = %s %d", PRO_GET_FUN_NAME, out, len);
		return len;
	}
    pro_log_error("%s:insufficient string length!", PRO_GET_FUN_NAME);
	return -2;
}


