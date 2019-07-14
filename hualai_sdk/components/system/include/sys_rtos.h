/*
 * @Description: rtos header file of system layer
 * @Author: Eric Zhang
 * @Date: 2019-06-23 14:48:21
 * @LastEditTime: 2019-07-04 19:55:16
 * @LastEditors: Please set LastEditors
 */
#ifndef __SYS_RTOS_H__
#define __SYS_RTOS_H__


/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_rtos_set_delay(unsigned long ms);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int sys_rtos_get_heap_size(void);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int sys_rtos_get_min_heap_size(void);


#endif
/*************************************************END****************************************************/