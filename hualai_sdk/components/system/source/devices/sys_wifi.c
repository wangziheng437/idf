/*
 * @Description: Basic support for wifi
 * @Author: your name
 * @Date: 2019-06-19 20:42:20
 * @LastEditTime: 2019-07-12 20:58:33
 * @LastEditors: Please set LastEditors
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_system.h"
//#include "internal/esp_wifi_internal.h"
#include "lwip/apps/sntp.h"

#include "sys_wifi.h"

/**
 * @description: Define constant
 */


/**
 * @description: Define variables
 */
typedef struct{
    int  ip_len;
    char ip[20];
}wifi_sta_ip_t;

EventGroupHandle_t wifi_event_group = NULL;
const int COMFIRM_CONNECTED_BIT = BIT0;
wifi_sta_ip_t sta_ip = {};

/**
 * @description: Function declaration
 */
esp_err_t event_handler(void *ctx, system_event_t *event);
int sys_country_code_nchan(char *code);

/**
 * @description: WiFi event handling callback function
 * @param {type} 
 * @return: 
 */
esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            sys_log_error("wifi station start");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
        
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            sta_ip.ip_len = snprintf(sta_ip.ip, 20, IPSTR, IP2STR(&event->event_info.got_ip.ip_info.ip));//get ip
            xEventGroupSetBits(wifi_event_group, COMFIRM_CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, COMFIRM_CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_STOP:
            sys_log_error("wifi station stop!");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            sys_log_info("station:"MACSTR" join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            sys_log_info("station:"MACSTR"leave, AID=%d",
                 MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);
            break;
        default:
            sys_log_error("wifi status unknown %d!", event->event_id);
            break;
    }
    return ESP_OK;
}

/**
 * @description: Confirm if Wi-Fi is connected successfully.If it fails, it will block the task.
 * @param {type} 
 * @return: 
 */
void sys_wifi_wait_connect(void)
{
    xEventGroupWaitBits(wifi_event_group, COMFIRM_CONNECTED_BIT, SYS_FALSE, SYS_TRUE, portMAX_DELAY);
}

/**
 * @description: configure for wifi
 * @param {type} 
 * @return: 
 */
int sys_wifi_config(sys_wifi_config_t *conf)
{
    uint8_t ssid_len = 0;
    uint8_t pass_len = 0;
    wifi_config_t wifi_config_sta = {};
    wifi_config_t wifi_config_ap  = {};
    
    memset(&wifi_config_sta,0,sizeof(wifi_config_t));
    memset(&wifi_config_ap,0,sizeof(wifi_config_t));
    
    if(conf == NULL)
    {
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
        return SYS_FAIL;
    }
    
    switch (conf->mode)
    {
        case SYS_WIFI_MODE_STA:
            ssid_len = strlen(conf->sta_ssid);
            pass_len = strlen(conf->sta_password);
            if(ssid_len > 32 || pass_len > 64)
            {
                sys_log_error("%s:wifi sta param error!", SYS_GET_FUN_NAME);
                return SYS_FAIL;
            }
            memcpy(wifi_config_sta.sta.ssid, conf->sta_ssid, ssid_len);
            memcpy(wifi_config_sta.sta.password, conf->sta_password, pass_len);

            sys_log_info("Setting WiFi SSID:%s", wifi_config_sta.sta.ssid);
            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
            ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_sta));
            break;
        case SYS_WIFI_MODE_AP:
            ssid_len = strlen(conf->ap_ssid);
            pass_len = strlen(conf->ap_password);
            if(ssid_len > 32 || pass_len > 64)
            {
                sys_log_error("%s:wifi ap param error!", SYS_GET_FUN_NAME);
                return SYS_FAIL;
            }
            memcpy(wifi_config_ap.ap.ssid, conf->ap_ssid, ssid_len);
            memcpy(wifi_config_ap.ap.password, conf->ap_password, pass_len);

            wifi_config_ap.ap.ssid_len = ssid_len;
            wifi_config_ap.ap.max_connection = conf->max_connection;
            wifi_config_ap.ap.channel = conf->channel;
            wifi_config_ap.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
            if(pass_len == 0)wifi_config_ap.ap.authmode = WIFI_AUTH_OPEN;
            
            sys_log_info("Setting WiFi AP:%s", conf->ap_ssid);
            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
            ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config_ap));
            break;
        case SYS_WIFI_MODE_APSTA:
            ssid_len = strlen(conf->sta_ssid);
            pass_len = strlen(conf->sta_password);
            if(ssid_len > 32 || pass_len > 64)
            {
                sys_log_error("%s:wifi sta param error!", SYS_GET_FUN_NAME);
                return SYS_FAIL;
            }
            memcpy(wifi_config_sta.sta.ssid, conf->sta_ssid, ssid_len);
            memcpy(wifi_config_sta.sta.password, conf->sta_password, pass_len);
     
            ssid_len = strlen(conf->ap_ssid);
            pass_len = strlen(conf->ap_password);
            if(ssid_len > 32 || pass_len > 64)
            {
                sys_log_error("%s:wifi ap param error!", SYS_GET_FUN_NAME);
                return SYS_FAIL;
            }
            memcpy(wifi_config_ap.ap.ssid, conf->ap_ssid, ssid_len);
            memcpy(wifi_config_ap.ap.password, conf->ap_password, pass_len);
            wifi_config_ap.ap.ssid_len       = ssid_len;
            wifi_config_ap.ap.max_connection = conf->max_connection;
            wifi_config_ap.ap.channel        = conf->channel;
            wifi_config_ap.ap.authmode       = WIFI_AUTH_WPA_WPA2_PSK;
            if(pass_len == 0)wifi_config_ap.ap.authmode = WIFI_AUTH_OPEN;
            
            sys_log_info("Setting WiFi SSID:%s, AP:%s", conf->sta_ssid, conf->ap_ssid);
            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
            ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config_ap));
            ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config_sta));
            break;
        default:
            sys_log_error("%s:no mode found %d!", SYS_GET_FUN_NAME, conf->mode);
            return SYS_FAIL;
            break;
    }

    ESP_ERROR_CHECK(esp_wifi_start());
    return SYS_OK;
}

/**
 * @description: Initialise wifi
 * @param {type} 
 * @return: 
 */
void sys_wifi_int(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    sys_log_info("WiFi init");
}

/**
 * @Descripttion: get ap rssi
 * @param {type} 
 * @return: 
 */
int8_t sys_wifi_get_rssi(void)
{
    #if(SYS_CORE_MODEL == SYS_ESP8266)
    return esp_wifi_get_ap_rssi();
    #elif(SYS_CORE_MODEL == SYS_ESP32)
    return -40;
    #endif

}

/**
 * @Descripttion: get sta ip
 * @param {type} 
 * @return: 
 */
char* sys_wifi_get_ip(void)
{
    if(sta_ip.ip_len < 17)return sta_ip.ip;
    return NULL;
}

/**
 * @Descripttion: 
 * @param {type} 
 * @return: 
 */
int sys_country_code_nchan(char *code)
{
    if(code != NULL && strlen(code) <3)
    {
        if (strcmp(code, "CN") == 0 || strcmp(code, "EU") ==0 || strcmp(code, "AU") == 0 || strcmp(code, "KR") == 0)return 13;
        if (strcmp(code, "US") == 0 || strcmp(code, "CA") ==0 || strcmp(code, "SG") == 0)return 11;
        if (strcmp(code, "FR") == 0)return 9;
        if (strcmp(code, "JP") == 0)return 14;
    }
    sys_log_error("%s:param error!", SYS_GET_FUN_NAME);
	return 0;	
}

/**
 * @Descripttion: 
 * @param {type} 
 * @return: 
 */
int sys_wifi_set_country(char *code)
{
	wifi_country_t config = {};

    config.nchan = sys_country_code_nchan(code);
    if(config.nchan > 0)
    {
        memcpy(config.cc, code, 2);
        config.schan=1;
        config.policy=WIFI_COUNTRY_POLICY_MANUAL;
        if(ESP_OK == esp_wifi_set_country(&config))return SYS_OK;
        sys_log_error("%s:set fail!", SYS_GET_FUN_NAME);
        return SYS_FAIL;
    }
	sys_log_error("%s:param error!", SYS_GET_FUN_NAME);
    return SYS_FAIL;
}

/**
 * @Descripttion: NTP timing initialization
 * @param {type} 
 * @return: 
 */
void sys_wifi_sntp_init(void)
{
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");	
    sntp_setservername(1, "us.ntp.org.cn");
    sntp_setservername(2, "cn.ntp.org.cn");
    sntp_init();
    sys_log_info("Sntp init");
}
