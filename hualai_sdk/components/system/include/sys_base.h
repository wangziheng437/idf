/*
 * @Description: header file of system layer
 * @Author: 
 * @Date: 2019-06-14 14:15:02
 * @LastEditTime: 2019-07-06 20:50:28
 */
#ifndef __SYS_BASE_H__
#define __SYS_BASE_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define SYS_TRUE             1
#define SYS_FALSE            0

#define SYS_OK               1
#define SYS_FAIL             0

#define SYS_GET_FUN_NAME      __FUNCTION__

#define SYS_LOG_ERROR         1
#define SYS_LOG_INFO          2
#define SYS_LOG_DEBUG         3

#define SYS_ESP32             1
#define SYS_ESP8266           2

#define SYS_LOG_LEVEL         SYS_LOG_DEBUG

#define SYS_CORE_MODEL        SYS_ESP32



/**
 * @description: 
 * @param {type} 
 * @return: 
 */
#if(SYS_LOG_LEVEL == SYS_LOG_ERROR)
#define sys_log_debug(info...)     
#define sys_log_info(info...) 
#define sys_log_error(info...)     {printf(info);printf("\n");}
#elif(SYS_LOG_LEVEL == SYS_LOG_INFO)
#define sys_log_debug(info...)     
#define sys_log_info(info...)      {printf(info);printf("\n");}
#define sys_log_error(info...)     {printf(info);printf("\n");}
#elif(SYS_LOG_LEVEL == SYS_LOG_DEBUG)
#define sys_log_debug(info...)     {printf(info);printf("\n");}
#define sys_log_info(info...)      {printf(info);printf("\n");}
#define sys_log_error(info...)     {printf(info);printf("\n");}
#else
#define sys_log_debug(info...) 
#define sys_log_info(info...)    
#define sys_log_error(info...)     {printf(info);printf("\n");}
#endif


#endif
/*************************************************END****************************************************/
