/*
 * @Description: header file of function layer
 * @Author: 
 * @Date: 2019-06-14 16:18:54
 * @LastEditTime: 2019-07-06 03:36:33
 */

#ifndef __FUN_DATA_H__
#define __FUN_DATA_H__



/**
 * @description:
 */


/**
 * @description: 
 */
typedef enum {
	FUN_IOT_STATE_INIT = 0,
    FUN_IOT_STATE_WORK,
    FUN_IOT_STATE_CERT,
    FUN_IOT_STATE_UPGRADE,
    FUN_IOT_STATE_DELETE,
} fun_data_iot_state_t;

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void fun_data_set_iot_state(char state);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
char fun_data_get_iot_state(void);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void fun_data_set_iot_timestamp(char *ts);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void fun_data_parse_iot_protocol(char *buf);



#endif
/*************************************************END****************************************************/