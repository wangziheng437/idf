/*
 * @Description:
 * @Author: 
 * @Date: 2019-06-13 14:43:13
 * @LastEditTime: 2019-07-06 19:28:07
 */
#include "pro_iot.h"
#include "fun_base.h"
#include "fun_data.h"

/**
 * @description: 
 */
typedef struct {
	char state;
    char ts[15];
}fun_data_iot_t;

/**
 * @description: 
 */
fun_data_iot_t data_iot = {};

/**
 * @description: 
 * @param: 
 * @return: 
 */



/**
 * @description: 
 * @param: 
 * @return: 
 */
void fun_data_set_iot_state(char state)
{
	data_iot.state = state;	
}

/**
 * @description: 
 * @param: 
 * @return: 
 */
char fun_data_get_iot_state(void)
{
	return	data_iot.state;
}

/**
 * @description: 
 * @param: 
 * @return: 
 */
void fun_data_set_iot_timestamp(char *ts)
{
    if(ts == NULL)
    {
        fun_log_error("%s:param NULL!", FUN_GET_FUN_NAME);
        return;  
    }
    memcpy(data_iot.ts, ts, 13);  
}

/**
 * @description: 
 * @param: 
 * @return: 
 */
void fun_data_parse_iot_protocol(char *buf)
{
    if(buf == NULL)
	{
        fun_log_error("%s:param NULL!", FUN_GET_FUN_NAME);
		return;
    }
	
    //解密
	
    pro_iot_rec_t  *iot_rec=(pro_iot_rec_t *)malloc(sizeof(pro_iot_rec_t));
	if(iot_rec == NULL)
	{
        fun_log_error("%s:malloc rec param fail!", FUN_GET_FUN_NAME);
		return;
    }
	bzero(iot_rec, sizeof(pro_iot_rec_t));
    if(PRO_CODE_SUCCECSS != pro_iot_receive_data(buf,iot_rec))
    {
        free(iot_rec);
        iot_rec = NULL;
        return;
    }

    if(strcmp(data_iot.ts, iot_rec->ts) >= 0) //Filter and retransmit data
    {
        fun_log_error("%s:stale data!", FUN_GET_FUN_NAME);
        free(iot_rec);
        iot_rec = NULL;
        return;
    }
    fun_data_set_iot_timestamp(iot_rec->ts);
    
    if(strcmp("upgrade", iot_rec->action) == 0)
    {
        pro_iot_upgrade_t *iot_upgrade=(pro_iot_upgrade_t *)malloc(sizeof(pro_iot_upgrade_t));
        if(iot_upgrade == NULL)
        {
            fun_log_error("%s:malloc up param fail!", FUN_GET_FUN_NAME);
            return;
        }
        bzero(iot_upgrade, sizeof(pro_iot_upgrade_t));
        if(PRO_CODE_SUCCECSS != pro_iot_params_upgrade(iot_rec->params, iot_upgrade))
        {
            free(iot_upgrade);
            iot_upgrade = NULL;
        }
        //存储iot_upgrade
        fun_data_set_iot_state(FUN_IOT_STATE_UPGRADE);   
    }
    else if(strcmp("delDev", iot_rec->action) == 0)
    {
        fun_data_set_iot_state(FUN_IOT_STATE_DELETE);
    }
    else
    {
        /* code */
    }
    
    free(iot_rec);
    iot_rec = NULL;
}
