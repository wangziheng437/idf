/*
 * @Description: 
 * @Author: 
 * @Date: 2019-06-13 15:23:46
 * @LastEditTime: 2019-07-06 04:59:17
 */

#include <sys/time.h>
#include "time.h"

#include "sys_base.h"

/**
 * @description: Define constant
 */


/**
 * @description: Define variables
 */
typedef struct{
    char ms[20];
    char date[80];
}sys_timestamp_t;

sys_timestamp_t stamp = {};

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
char *sys_time_stamp(void)
{
    int len = 0;
    struct timeval tv;     //esp-idf/components/newlib/include/sys/time.h
    
    gettimeofday(&tv, NULL);    
    len = sprintf(stamp.ms, "%.010ld%.03ld", tv.tv_sec, tv.tv_usec/1000);
    if(len >= sizeof(stamp.ms))
    {
        memcpy(stamp.ms, stamp.ms, 13);
        stamp.ms[13] = '\0';
    }
    return stamp.ms;
}

/**
 * @description: display time by format of year-month-day hour:minute:second.millisecond
 * @param: void
 * @return: char
 */
char *sys_time_date(void)
{
    int len = 0;
    time_t tim;
    struct timeval tv;
    
    gettimeofday(&tv, NULL); 
    time(&tim);
    len = strftime(stamp.date, sizeof(stamp.date),"%Y-%m-%d %H:%M:%S.", localtime(&tim));
    len = sprintf(stamp.date + len, "%.03ld", tv.tv_usec/1000);
    if(len >= sizeof(stamp.date))
    {
        memcpy(stamp.date, stamp.date, 23);
        stamp.date[23] = '\0';
    }
    return stamp.date;
}
