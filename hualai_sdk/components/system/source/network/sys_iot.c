/*
 * @Description: Basic support for AWS IoT
 * @Author: 
 * @Date: 2019-06-14 16:37:30
 * @LastEditTime: 2019-07-04 20:09:56
 */

#include "aws_iot_mqtt_client_interface.h"
#include "aws_iot_mqtt_client.h"
#include "aws_iot_version.h"

#include "sys_base.h"
#include "sys_iot.h"

/**
 * @description: 
 */
#define SUB_QOS_GRADE    QOS1
#define PUB_QOS_GRADE    QOS1

/**
 * @description: 
 */
AWS_IoT_Client client;
sys_iot_info_cb_fun sys_iot_info_cb;
sys_iot_info_t iot_info = {};


/**
 * @description: 
 */
void sys_iot_info_execute_cb(sys_iot_info_t *info);
void sys_iot_subscribe_callback1(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
                                    IoT_Publish_Message_Params *params, void *pData);
void sys_iot_subscribe_callback2(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
                                    IoT_Publish_Message_Params *params, void *pData);
void sys_iot_subscribe_callback3(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
                                    IoT_Publish_Message_Params *params, void *pData);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_iot_info_sign_cb(sys_iot_info_cb_fun fun)
{
	sys_iot_info_cb = fun;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_iot_info_execute_cb(sys_iot_info_t *info)
{	
    if(sys_iot_info_cb == NULL)
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        return;
    }
	sys_iot_info_cb(info);
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_iot_subscribe_callback1(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
                                    IoT_Publish_Message_Params *params, void *pData)
{
	iot_info.sub_data_len1 = (int)params->payloadLen;
	iot_info.sub_data1 = params->payload;
	iot_info.step = SYS_IOT_CB_STEP_SUBCB1;
	sys_iot_info_execute_cb(&iot_info);
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_iot_subscribe_callback2(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
                                    IoT_Publish_Message_Params *params, void *pData)
{
	iot_info.sub_data_len2 = (int)params->payloadLen;
	iot_info.sub_data2 = params->payload;
	iot_info.step = SYS_IOT_CB_STEP_SUBCB2;
	sys_iot_info_execute_cb(&iot_info);
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_iot_subscribe_callback3(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen,
                                    IoT_Publish_Message_Params *params, void *pData)
{
	iot_info.sub_data_len3 = (int)params->payloadLen;
	iot_info.sub_data3 = params->payload;
	iot_info.step = SYS_IOT_CB_STEP_SUBCB3;
	sys_iot_info_execute_cb(&iot_info);
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_iot_reconnect_callback(AWS_IoT_Client *pClient, void *data)
{
	iot_info.step = SYS_IOT_CB_STEP_RECON;
    if(NULL == pClient)
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        iot_info.code = NULL_VALUE_ERROR;
        sys_iot_info_execute_cb(&iot_info);
        return;
    }

	sys_log_info("aws iot reconnect start!");
    if(aws_iot_is_autoreconnect_enabled(pClient))
    {
        sys_log_error("reconnecting aws iot attempt will start now");
        iot_info.code = SYS_IOT_AUTO_RECONNECT;
    }
    else
    {
		sys_log_error("starting manual reconnect aws iot");
        iot_info.code = SYS_IOT_MANUAL_RECONNECT;
    }
	
    sys_iot_info_execute_cb(&iot_info);
}

/**
 * @description: Initialise aws iot
 * @param {type} 
 * @return: 
 */
void sys_iot_init(char *url,int port,char *crt_root,char *crt_crt,char *crt_key)
{
	IoT_Error_t rc = FAILURE;
	IoT_Client_Init_Params mqttInitParams = iotClientInitParamsDefault;
    
	iot_info.step = SYS_IOT_CB_STEP_INIT;

    if((url == NULL) || (port <= 0) || (crt_root == NULL) || (crt_crt == NULL) || (crt_key == NULL))
    {
		sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        iot_info.code = NULL_VALUE_ERROR;
        sys_iot_info_execute_cb(&iot_info);
        return;
    }
    
	sys_log_info("aws iot sdk ver %d.%d.%d-%s", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_TAG);

	mqttInitParams.enableAutoReconnect = 0;//SYS_FALSE;
	mqttInitParams.pHostURL = url;
	mqttInitParams.port = port;

	mqttInitParams.pRootCALocation = crt_root;
	mqttInitParams.pDeviceCertLocation = crt_crt;
	mqttInitParams.pDevicePrivateKeyLocation = crt_key;

	mqttInitParams.mqttCommandTimeout_ms = 20000;
	mqttInitParams.tlsHandshakeTimeout_ms = 50000;
	// mqttInitParams.isSSLHostnameVerify = SYS_FALSE;
	mqttInitParams.isSSLHostnameVerify = SYS_TRUE;
	mqttInitParams.disconnectHandler = sys_iot_reconnect_callback;
	mqttInitParams.disconnectHandlerData = NULL;

	if(SUCCESS != (rc = aws_iot_mqtt_init(&client, &mqttInitParams)))
	{
		sys_log_error("%s:abnormal %d!", SYS_GET_FUN_NAME, rc);
	}
    
    iot_info.code = rc;
    sys_iot_info_execute_cb(&iot_info);
}

/**
 * @description: Reconnect aws iot
 * @param {type} 
 * @return: 
 */
void sys_iot_reinit(char *crt_root,char *crt_crt,char *crt_key)
{
	iot_info.step = SYS_IOT_CB_STEP_INIT;

    if(crt_root == NULL || crt_crt == NULL || crt_key == NULL)
    {
		sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        iot_info.code = NULL_VALUE_ERROR;
        sys_iot_info_execute_cb(&iot_info);
        return;
    }
	client.networkStack.tlsConnectParams.pRootCALocation = crt_root;
	client.networkStack.tlsConnectParams.pDeviceCertLocation = crt_crt;
	client.networkStack.tlsConnectParams.pDevicePrivateKeyLocation = crt_key;
	
	iot_info.code = SUCCESS;
    sys_iot_info_execute_cb(&iot_info);	
}

/**
 * @description: Connect aws iot
 * @param {type} 
 * @return: 
 */
void sys_iot_connect(int keep_alive,char *client_id)
{
	IoT_Error_t rc=FAILURE;
	IoT_Client_Connect_Params connectParams = iotClientConnectParamsDefault;

	iot_info.step = SYS_IOT_CB_STEP_CON;

    if(client_id == NULL)
    {
		sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        iot_info.code = NULL_VALUE_ERROR;
        sys_iot_info_execute_cb(&iot_info);
        return;
    }
    
	if(keep_alive < 30)keep_alive = 30;
	connectParams.keepAliveIntervalInSec = keep_alive;
	connectParams.isCleanSession = SYS_TRUE;
	connectParams.MQTTVersion = MQTT_3_1_1;
	connectParams.pClientID = client_id;
	connectParams.clientIDLen = (uint16_t) strlen(client_id);
	connectParams.isWillMsgPresent = SYS_FALSE;

    sys_log_error("aws iot connecting...");
    
	if(SUCCESS != (rc=aws_iot_mqtt_connect(&client, &connectParams)))
	{
		sys_log_error("%s:abnormal %d!", SYS_GET_FUN_NAME, rc);
	}
	
	iot_info.code = rc;
    sys_iot_info_execute_cb(&iot_info);
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_iot_subscribe(char *sub_topic, int sub_len, int fun)
{
	IoT_Error_t rc = FAILURE;

	iot_info.step = SYS_IOT_CB_STEP_SUB;
	
    if((sub_topic == NULL) || (sub_len <= 0))
    {
		sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        iot_info.code = NULL_VALUE_ERROR;
        sys_iot_info_execute_cb(&iot_info);
		return;
    }

	if(fun == 1)
	{
		rc = aws_iot_mqtt_subscribe(&client,(const char *)sub_topic,sub_len,SUB_QOS_GRADE,sys_iot_subscribe_callback1,NULL);
	}
	else if(fun == 2)
	{
		rc = aws_iot_mqtt_subscribe(&client,(const char *)sub_topic,sub_len,SUB_QOS_GRADE,sys_iot_subscribe_callback2,NULL);
	}
	else if(fun == 3)
	{
		rc = aws_iot_mqtt_subscribe(&client,(const char *)sub_topic,sub_len,SUB_QOS_GRADE,sys_iot_subscribe_callback3,NULL);
	}
	else
	{
		sys_log_error("%s:param unknown!", SYS_GET_FUN_NAME);
        iot_info.code = NULL_VALUE_ERROR;
        sys_iot_info_execute_cb(&iot_info);
		return;
	}
					
	if(SUCCESS != rc)
	{
		sys_log_error("%s:abnormal %d!", SYS_GET_FUN_NAME, rc);
	}
    
	iot_info.code = rc;
    sys_iot_info_execute_cb(&iot_info);
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_iot_resubscribe(void)
{
	IoT_Error_t rc=FAILURE;
	
	if(SUCCESS != (rc=aws_iot_mqtt_resubscribe(&client)))
	{
		sys_log_error("%s:abnormal %d!", SYS_GET_FUN_NAME, rc);
	}
	
	iot_info.step = SYS_IOT_CB_STEP_RESUB;
	iot_info.code = rc;
    sys_iot_info_execute_cb(&iot_info);
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_iot_unsubscribe(char *sub_topic, int sub_len)
{
	IoT_Error_t rc=FAILURE;
	
	iot_info.step = SYS_IOT_CB_STEP_UNSUB;
	
    if((sub_topic == NULL) || (sub_len <= 0))
    {
		sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        iot_info.code = NULL_VALUE_ERROR;
        sys_iot_info_execute_cb(&iot_info);
		return;
    }

	if(SUCCESS != (rc=aws_iot_mqtt_unsubscribe(&client,(const char *)sub_topic,sub_len)))
	{
		sys_log_error("%s:abnormal %d!", SYS_GET_FUN_NAME, rc);
	}
	
	iot_info.code = rc;
    sys_iot_info_execute_cb(&iot_info);
}

/**
 * @Descripttion: 
 * @param {type} 
 * @return: 
 */
void sys_iot_publish(char *pub_topic, int pub_len, char *payload, int payload_len)
{
	IoT_Error_t rc = FAILURE;
	IoT_Publish_Message_Params paramsQOS;
	
	iot_info.step = SYS_IOT_CB_STEP_PUB;
	bzero(&paramsQOS,sizeof(IoT_Publish_Message_Params));
    if(pub_topic == NULL || pub_len <= 0 || payload == NULL || payload_len <= 0)
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        iot_info.code = NULL_VALUE_ERROR;
        sys_iot_info_execute_cb(&iot_info);
    }

	paramsQOS.qos = PUB_QOS_GRADE;
	paramsQOS.payload = (void *) payload;
	paramsQOS.payloadLen = payload_len;
	paramsQOS.isRetained = 0;

	if(SUCCESS != (rc=aws_iot_mqtt_publish(&client,(const char *)pub_topic,pub_len,&paramsQOS)))
	{
		sys_log_error("%s:abnormal %d!", SYS_GET_FUN_NAME, rc);
	}
	iot_info.code = rc;
    sys_iot_info_execute_cb(&iot_info);
}

/**
 * @Descripttion: disconnect MQTT
 * @param {type} 
 * @return: 
 */
void sys_iot_disconnect(void)
{
	IoT_Error_t rc = FAILURE;
	
	iot_info.step = SYS_IOT_CB_STEP_DISCON;
	if(SUCCESS != (rc=aws_iot_mqtt_disconnect(&client)))
	{
		sys_log_error("%s:abnormal %d!", SYS_GET_FUN_NAME, rc);
	}
	else
	{
		sys_log_info("aws iot disconnect!");
	}
	
	iot_info.code = rc;
    sys_iot_info_execute_cb(&iot_info);
}

/**
 * @Descripttion: 
 * @param {type} 
 * @return: 
 */
int sys_iot_yield(int timeout_ms)
{
	IoT_Error_t rc = FAILURE;
	
	rc=aws_iot_mqtt_yield(&client,timeout_ms);
	if(NETWORK_ATTEMPTING_RECONNECT == rc)
	{
		return SYS_FALSE;
	}
	return SYS_TRUE;
}

