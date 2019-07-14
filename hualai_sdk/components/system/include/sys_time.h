/*
 * @Description: time header file of system layer
 * @Author: Eric Zhang
 * @Date: 2019-06-23 14:48:21
 * @LastEditTime: 2019-07-02 18:56:25
 * @LastEditors: Please set LastEditors
 */
#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__


/**
 * @description: Get a 13-bit timestamp
 * @param {void} 
 * @return: string
 */
char *sys_time_stamp(void);

/**
 * @description: Get a timestamp in calendar form
 * @param {void} 
 * @return: string
 */
char *sys_time_date(void);



#endif
/*************************************************END****************************************************/