/*
 * @Description: header file of function layer
 * @Author: 
 * @Date: 2019-06-14 16:18:54
 * @LastEditTime: 2019-07-04 18:13:25
 */

#ifndef __FUN_BASE_H__
#define __FUN_BASE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "sys_time.h"

/**
 * @description:
 */
#define FUN_TRUE             1
#define FUN_FALSE            0

#define FUN_OK               1
#define FUN_FAIL             0

#define FUN_GET_FUN_NAME      __FUNCTION__

#define FUN_LOG_ERROR         1
#define FUN_LOG_INFO          2
#define FUN_LOG_DEBUG         3

#define FUN_LOG_LEVEL         FUN_LOG_INFO

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
#if(FUN_LOG_LEVEL == FUN_LOG_ERROR)
#define fun_log_debug(info...)     
#define fun_log_info(info...) 
#define fun_log_error(info...)     {printf("%s ", sys_time_date());printf(info);printf("\n");}
#elif(FUN_LOG_LEVEL == FUN_LOG_INFO)
#define fun_log_debug(info...) 
#define fun_log_info(info...)      {printf("%s ", sys_time_date());printf(info);printf("\n");}
#define fun_log_error(info...)     {printf("%s ", sys_time_date());printf(info);printf("\n");}
#elif(PRO_LOG_LEVEL == FUN_LOG_DEBUG)
#define fun_log_debug(info...)     {printf("%s ", sys_time_date());printf(info);printf("\n");}
#define fun_log_info(info...)      {printf("%s ", sys_time_date());printf(info);printf("\n");}
#define fun_log_error(info...)     {printf("%s ", sys_time_date());printf(info);printf("\n");}
#else
#define fun_log_debug(info...)     
#define fun_log_info(info...) 
#define fun_log_error(info...)     {printf("%s ", sys_time_date());printf(info);printf("\n");}
#endif

#endif
/*************************************************END****************************************************/