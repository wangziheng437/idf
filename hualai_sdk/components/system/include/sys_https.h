/*
 * @Description: https header file of system layer
 * @Author: your name
 * @Date: 2019-06-23 14:48:21
 * @LastEditTime: 2019-07-08 19:39:41
 * @LastEditors: Please set LastEditors
 */
#ifndef __SYS_HPPTS_H__
#define __SYS_HPPTS_H__

#include "sys_base.h"

/**
 * @description: 
 */
typedef enum {
    SYS_HTTPS_CB_STEP_INIT = 1,  
    SYS_HTTPS_CB_STEP_CON, 
    SYS_HTTPS_CB_STEP_WRITE,       
    SYS_HTTPS_CB_STEP_READ, 
    SYS_HTTPS_CB_STEP_AGAIN,       
    SYS_HTTPS_CB_STEP_EXIT,	
}sys_https_step_t;

typedef enum {
	SYS_HTTPS_CODE_PARAM_NULL = -5,
	SYS_HTTPS_CODE_LENGTH_ERROR = -4,
	SYS_HTTPS_CODE_TYPE_ERROR = -3,
	SYS_HTTPS_CODE_VALUE_ERROR = -2,
	SYS_HTTPS_CODE_VALUE_NULL = -1, 
    SYS_HTTPS_CODE_SUCCESS = 0,  
}sys_https_code_t;

typedef struct {
    int step;
	int code;	
}sys_https_info_t;

typedef void (*sys_https_info_cb_fun)(sys_https_info_t *);


#endif
/*************************************************END****************************************************/