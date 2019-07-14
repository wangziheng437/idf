/*
 * @Description: 
 * @Author: 
 * @Date: 2019-06-14 16:37:30
 * @LastEditTime: 2019-07-04 19:53:25
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "sys_base.h"

/**
 * @description:  
 * @param {type} 
 * @return: 
 */
void sys_rtos_set_delay(unsigned long ms)
{
    vTaskDelay(ms / portTICK_RATE_MS);
}

/**
 * @description:  
 * @param {type} 
 * @return: 
 */
int sys_rtos_get_heap_size(void)
{
    return esp_get_free_heap_size();
}

/**
 * @description:  
 * @param {type} 
 * @return: 
 */
int sys_rtos_get_min_heap_size(void)
{
    return esp_get_minimum_free_heap_size();
}

