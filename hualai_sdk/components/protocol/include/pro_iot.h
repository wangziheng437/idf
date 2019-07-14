/*
 * @Description: iot header file of protocol layer
 * @Author: Eric Zhang
 * @Date: 2019-06-23 14:48:21
 * @LastEditTime: 2019-07-08 03:33:15
 * @LastEditors: Please set LastEditors
 */
#ifndef __PRO_IOT_H__
#define __PRO_IOT_H__

/**
 * @description: 
 */
typedef enum {
	PRO_CODE_SUCCECSS = 1,
	PRO_CODE_PARAM_NULL = -1,
	PRO_CODE_LENGTH_ERROR = -2,
	PRO_CODE_TYPE_ERROR = -3,
	PRO_CODE_VALUE_ERROR = -4,
	PRO_CODE_VALUE_NULL = -5,
}pro_iot_error_code_t;

typedef struct {
	char action[50];
	char params[1024];
    char guid[50];
    char ts[20];
}pro_iot_rec_t;

typedef struct {
	char mac[15];
	char model[20];
	char data[512];
    char ts[20];
}pro_iot_send_t;

typedef struct {
	char url[512];
	char md5[50];
	char model[20];
}pro_iot_upgrade_t;

/**
 * @description: Parse the received IOT data
 * @param {type} 
 *          -buf input string
 *          -out output structure pointer
 * @return: 
 *          -0 fail
 *          -1 success
 */
int pro_iot_receive_data(char *buf, pro_iot_rec_t *out);

/**
 * @description: Parsing upgrade parameter data
 * @param {type} 
 *          -param input string
 *          -out output structure pointer
 * @return: 
 *          -0 fail
 *          -1 success 
 */
int pro_iot_params_upgrade(char *param, pro_iot_upgrade_t *out);

/**
 * @Descripttion: Generate published ack data
 * @param {type} 
 *          -guid input string
 *          -result input numerical value 1 or 0
 *          -out_len input out string total length
 *          -out output string
 * @return: 
 *          -0 fail
 *          -1 success 
 */
int pro_iot_send_ack(char *guid, char result, char *out, int out_len);

/**
 * @Descripttion: Generate published reporting data
 * @param {type} 
 *          -in input structure pointer
 *          -out_len input out string total length
 *          -out output string
 * @return: 
 *          -0 fail
 *          -1 success 
 */
int pro_iot_send_data(pro_iot_send_t *in, char *out, int out_len);

/**
 * @Descripttion: Select and generate the specified topic
 * @param {type} 
 *          -topic input topic code 0-3 
 *          -client_id input string
 *          -out_len input out string total length
 *          -out output string
 * @return: 
 *          -0 fail
 *          -1 success 
 */
int pro_iot_topic_choice(int topic, char *client_id, char *out, int out_len);


#endif
/*************************************************END****************************************************/