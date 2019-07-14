/*
 * @Descripttion: 
 * @Date: 2019-07-10 14:14:50
 * @LastEditTime: 2019-07-14 16:46:20
 */
#ifndef __APP_BLUFI_H__
#define __APP_BLUFI_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_blufi_api.h"

#define TRUE                  1
#define FALSE                 0

#define TKOEN_LEN           200
#define TIME_ZONE_LEN       10
#define COUNTRY_CODE_LEN    10
#define TIME_ZONE_LOCAL_LEN 50

#define CLIE_ODER     108
#define SERV_ODER     109

#define BLUFI_EXAMPLE_TAG "BLUFI_EXAMPLE"
#define BLUFI_INFO(fmt, ...)   ESP_LOGI(BLUFI_EXAMPLE_TAG, fmt, ##__VA_ARGS__) 
#define BLUFI_ERROR(fmt, ...)  ESP_LOGE(BLUFI_EXAMPLE_TAG, fmt, ##__VA_ARGS__) 

void blufi_dh_negotiate_data_handler(uint8_t *data, int len, uint8_t **output_data, int *output_len, bool *need_free);
int blufi_aes_encrypt(uint8_t iv8, uint8_t *crypt_data, int crypt_len);
int blufi_aes_decrypt(uint8_t iv8, uint8_t *crypt_data, int crypt_len);
uint16_t blufi_crc_checksum(uint8_t iv8, uint8_t *data, int len);

int blufi_security_init(void);
void blufi_security_deinit(void);

void app_blufi_proc(void);
TaskHandle_t pvIotTaskHandle;
void get_task_stack(int num);
void Confirm_wifi_connect_success(void);

/**
 * @Descripttion: analysis blu info
 * @param {type} 
 * @return: 
 */
char app_blu_analysis(char *buf,int len);

typedef struct {
	char head[2];
	int  version;
	int  order;
	int  length;
	int  token_len;
	char token[TKOEN_LEN];
	int  time_zone_len;
	char time_zone[TIME_ZONE_LEN];
	int  country_code_len;
	char country_code[COUNTRY_CODE_LEN];
    int  time_code_local_len;
    char time_code_local[TIME_ZONE_LOCAL_LEN];
}blu_info_t;

#endif