/*
 * @Description: base header file of protocol layer
 * @Author: Eric Zhang
 * @Date: 2019-06-23 14:48:21
 * @LastEditTime: 2019-07-06 02:54:53
 * @LastEditors: Please set LastEditors
 */
#ifndef __PRO_BASE_H__
#define __PRO_BASE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * @description:
 */
#define PRO_TRUE             1
#define PRO_FALSE            0

// #define PRO_OK               1
// #define PRO_FAIL             0

#define PRO_GET_FUN_NAME      __FUNCTION__

#define PRO_LOG_ERROR         1
#define PRO_LOG_DEBUG         2

#define PRO_LOG_LEVEL         PRO_LOG_DEBUG


/**
 * @description: 
 * @param {type} 
 * @return: 
 */
#if(PRO_LOG_LEVEL == PRO_LOG_ERROR)
#define pro_log_debug(info...)     
#define pro_log_error(info...)     {printf(info);printf("\n");}
#elif(PRO_LOG_LEVEL == PRO_LOG_DEBUG)
#define pro_log_debug(info...)     {printf(info);printf("\n");}
#define pro_log_error(info...)     {printf(info);printf("\n");}
#else
#define pro_log_debug(info...)     
#define pro_log_error(info...)     {printf(info);printf("\n");}
#endif


#endif
/*************************************************END****************************************************/