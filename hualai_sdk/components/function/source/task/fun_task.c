/*
 * @Description: 
 * @Author: your name
 * @Date: 2019-06-16 22:44:32
 * @LastEditTime: 2019-07-14 16:53:51
 * @LastEditors: Please set LastEditors
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "app_blufi.h"

#include "sys_rtos.h"
#include "sys_wifi.h"
#include "sys_flash.h"
#include "fun_base.h"
#include "fun_iot.h"

/**
 * @description: 
 */
TaskHandle_t pvIotTaskHandle;

/**
 * @Descripttion: 
 * @param {type} 
 * @return: 
 */
static void blufi(void *pvParameters)
{
    printf("************************the begin of blufi task**************************\n");
    app_blufi_proc();
    printf("************************the end of blufi task**************************\n");
    vTaskDelete(NULL);
}


/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void fun_task_iot(void *pvParameters)
{

    // int stack = uxTaskGetStackHighWaterMark(pvIotTaskHandle);
    // printf("----task stack %d\n",stack);
    
    fun_iot_work_function();
}


/**
 * @Descripttion: 
 * @param {type} 
 * @return: 
 */
void fun_task_set_info(void)
{
    printf("----task stack 0 %d\n",sys_rtos_get_heap_size());
    sys_flash_iot_crt_t *crt = malloc(sizeof(sys_flash_iot_crt_t));
    bzero(crt, sizeof(sys_flash_iot_crt_t));
    if(crt != NULL)
    {
        int root_len = strlen(IOT_AWS_ROOT_PEM);
        int crt_len  = strlen(IOT_CERTIFICATE_CRT);
        int key_len  = strlen(IOT_PRIVATE_PEM_KEY);
        printf("------------len %d\n", root_len);
        if((root_len < sizeof(crt->root)) && (crt_len < sizeof(crt->crt)) && (key_len < sizeof(crt->key)))
        {
            memcpy(crt->root, IOT_AWS_ROOT_PEM, root_len);
            memcpy(crt->crt, IOT_CERTIFICATE_CRT, crt_len);
            memcpy(crt->key, IOT_PRIVATE_PEM_KEY, key_len);
            
        } 
        printf("----task stack 1 %d\n",sys_rtos_get_heap_size());
        sys_flash_set_iot_crt(crt);
        printf("----task stack 2 %d\n",sys_rtos_get_heap_size());

        free(crt);
    }
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void fun_task_create(void)
{
    #if 0
    sys_wifi_config_t wifi_config = {
        .mode = SYS_WIFI_MODE_STA,
        .sta_ssid = "eeromesh",
        .sta_password = "123456789",
    };
    sys_flash_int();
    sys_wifi_int();
    sys_wifi_config(&wifi_config);
    #endif

    //fun_task_set_info();

    printf("\n\n\n\n********************begin main, all stack left: %d**********************\n\n\n\n\n", esp_get_free_heap_size());
    
    xTaskCreate(&blufi, "blufi", 5000, NULL, 5, pvIotTaskHandle);

    // xTaskCreate(&fun_task_iot, "fun_task_iot", 8192, NULL, 5, &pvIotTaskHandle);

    while(1)
    {
        vTaskDelay(10000 / portTICK_RATE_MS);
        printf("********************all stack left: %d**********************\n", esp_get_free_heap_size());
    }
}
