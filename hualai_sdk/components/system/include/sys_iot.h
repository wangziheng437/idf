/*
 * @Description: Iot header file of system layer
 * @Author: 
 * @Date: 2019-06-21 10:25:18
 * @LastEditTime: 2019-07-06 02:37:13
 */
#ifndef __SYS_IOT_H__
#define __SYS_IOT_H__

#include "sys_base.h"


/**
 * @description: callback
 */
typedef enum {
    SYS_IOT_CB_STEP_INIT = 1,  
    SYS_IOT_CB_STEP_CON, 
	SYS_IOT_CB_STEP_RECON,        
    SYS_IOT_CB_STEP_DISCON,        
    SYS_IOT_CB_STEP_SUB,     
    SYS_IOT_CB_STEP_RESUB,
	SYS_IOT_CB_STEP_UNSUB,
	SYS_IOT_CB_STEP_SUBCB1,
	SYS_IOT_CB_STEP_SUBCB2,
	SYS_IOT_CB_STEP_SUBCB3,
	SYS_IOT_CB_STEP_PUB,	
}sys_iot_step_t;

typedef enum {
	SYS_IOT_AUTO_RECONNECT = 29,
	SYS_IOT_MANUAL_RECONNECT = 28, 
    SYS_IOT_CODE_SUCCESS = 0,  
	SYS_IOT_ROOT_CRT_ERROR = -19,
	SYS_IOT_DEVICE_CRT_ERROR = -20,
	SYS_IOT_PRIVATE_KEY_ERROR = -21,
    SYS_IOT_REQUEST_TIMEOUT = -28, 
}sys_iot_code_t;

typedef struct{
    int step;
	int code;	//return IoT Error enum codes
    char * sub_data1;
    int sub_data_len1;
	char * sub_data2;
    int sub_data_len2;
	char * sub_data3;
    int sub_data_len3;
}sys_iot_info_t;

typedef void (*sys_iot_info_cb_fun)(sys_iot_info_t *);


/**
 * @description: Initialise AWS IOT
 * @param {type} 
 * 			-iot server address
 * 			-iot server port
 * 			-iot server certificate
 * @return: void
 */
void sys_iot_init(char *url,int port,char *crt_root,char *crt_crt,char *crt_key);

/**
 * @description: Reconnect initialise AWS IOT,after offline
 * @param {type} 
 * 			-iot server certificate
 * @return: void
 */
void sys_iot_reinit(char *crt_root,char *crt_crt,char *crt_key);

/**
 * @description: Connect AWS IOT
 * @param {type} 
 * 			-keep alive,uint sec
 * 			-client id
 * @return: void
 */
void sys_iot_connect(int keep_alive,char *client_id);

/**
 * @description: Subscribe to AWS IOT
 * @param {type} 
 * 			-subscribe topic
 * 			-subscribe topic length
 *          -subscription callback function label,value 1-3
 * @return: void
 */
void sys_iot_subscribe(char *sub_topic, int sub_len, int fun);

/**
 * @description: Resubscribe to AWS IOT,after offline
 * @param {void} 
 * @return: void
 */
void sys_iot_resubscribe(void);

/**
 * @description: Unsubscribe to AWS IOT topic,must be a previously subscribed topic
 * @param {type} 
 * 			-subscribe topic
 * 			-subscribe topic length
 * @return: void
 */
void sys_iot_unsubscribe(char *sub_topic, int sub_len);

/**
 * @description: Publish data to AWS IOT
 * @param {type} 
 * 			-publish topic
 * 			-publish topic length
 * 			-publish data content
 * 			-publish data length
 * @return: void
 */
void sys_iot_publish(char *pub_topic, int pub_len,char *payload, int payload_len);

/**
 * @description: Publish heartbeat to AWS IOT,need to be scanned all the time
 * @param {int} Single waiting time, unit ms
 * @return: FALSE IOT offline,TRUE IOT online
 */
int sys_iot_yield(int timeout_ms);

/**
 * @description: Disconnect AWS IOT connection and release memory
 * @param {void} 
 * @return: void
 */
void sys_iot_disconnect(void);

/**
 * @description: Register all IOT event callbacks
 * @param {type} sys_iot_info_cb_fun
 * @return: void
 */
void sys_iot_info_sign_cb(sys_iot_info_cb_fun fun);


#endif
/*************************************************END****************************************************/