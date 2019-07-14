/*
 * @Description: wifi header file of system layer
 * @Author: Eric Zhang
 * @Date: 2019-06-23 14:48:21
 * @LastEditTime: 2019-07-06 04:56:57
 * @LastEditors: Please set LastEditors
 */
#ifndef __SYS_WIFI_H__
#define __SYS_WIFI_H__

#include "sys_base.h"

/**
 * @description:
 */
typedef enum {
    SYS_WIFI_MODE_NULL = 0,  //null mode
    SYS_WIFI_MODE_STA,       //WiFi station mode 
    SYS_WIFI_MODE_AP,        //WiFi soft-AP mode
    SYS_WIFI_MODE_APSTA,     //WiFi station + soft-AP mode
    SYS_WIFI_MODE_MAX
}sys_wifi_mode_t;

typedef struct {
    sys_wifi_mode_t mode;    //mode for WIFI
    char sta_ssid[32];      
    char sta_password[64];  
    char ap_ssid[32];       
    char ap_password[64];   
    int  max_connection;     //Max number of stations allowed to connect in WIFI soft-AP
    int  channel;            //Channel of WIFI soft-AP 
}sys_wifi_config_t;

/**
 * @description: Initialise wifi
 * @param {void} 
 * @return: void
 */
void sys_wifi_int(void);

/**
 * @description: Configure WiFi as the specified mode
 * @param {sys_wifi_config_t} 
 * @return: 
 *      -1 ok
 *      -0 fail
 */
int sys_wifi_config(sys_wifi_config_t *conf);

/**
 * @description: If WiFi is not connected, it will block
 * @param {void} 
 * @return: void
 */
void sys_wifi_wait_connect(void);

/**
 * @Descripttion: Get RSSI for connecting AP
 * @param {void} 
 * @return: rssi generally negative
 */
int8_t sys_wifi_get_rssi(void);

/**
 * @Descripttion: Get station IP
 * @param {void} 
 * @return: Maximum 16 bytes string
 */
char* sys_wifi_get_ip(void);

/**
 * @Descripttion: Setting up Wifi channels in different countries
 * @param {string} 2 bytes 
 * @return: 
 *             -1 OK 
 *             -0 FAIL
 */
int sys_wifi_set_country(char *code);

/**
 * @Descripttion: NTP proofreading time
 * @param {void} 
 * @return: void
 */
void sys_wifi_sntp_init(void);

#endif
/*************************************************END****************************************************/