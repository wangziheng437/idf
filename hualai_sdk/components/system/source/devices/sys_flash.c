/*
 * @Description: Basic support for flash
 * @Author: your name
 * @Date: 2019-06-20 11:54:02
 * @LastEditTime: 2019-07-06 20:21:58
 * @LastEditors: Please set LastEditors
 */
#include "sys_base.h"
#include "sys_flash.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_err.h"
#if(SYS_CORE_MODEL == SYS_ESP8266)
#include "spi_flash.h"
#elif(SYS_CORE_MODEL == SYS_ESP32)
#include "esp_spi_flash.h"
#endif

/**
 * @description: initialise flash
 */
#if(SYS_CORE_MODEL == SYS_ESP8266)
#define FLASH_BASE_SECTOR   257
#elif(SYS_CORE_MODEL == SYS_ESP32)
#define FLASH_BASE_SECTOR   16
#endif

/**
 * @description:
 */
sys_flash_iot_crt_t *falsh_crt = NULL;

/**
 * @description:
 */
int sys_flash_get_flash_crt(sys_flash_iot_crt_t *crt);

/**
 * @description: initialise flash
 * @param {type} 
 * @return: 
 */
void sys_flash_int(void)
{
    nvs_flash_init();
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int sys_flash_set_iot_crt(sys_flash_iot_crt_t *crt)
{
	esp_err_t rc = ESP_FAIL;

	if(crt == NULL)
	{
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
		return SYS_FAIL;
	}
    
    int root_len = strlen(crt->root);
    int crt_len  = strlen(crt->crt);
    int key_len  = strlen(crt->key);
    
    if((root_len < sizeof(crt->root)) && (crt_len < sizeof(crt->crt)) && (key_len < sizeof(crt->key)))
    {
        sys_log_debug("%s:crt len %d %d %d", SYS_GET_FUN_NAME, root_len, crt_len, key_len);
        sys_log_debug("%s:crt.root %s", SYS_GET_FUN_NAME, crt->root);
        sys_log_debug("%s:crt.crt %s", SYS_GET_FUN_NAME, crt->crt);
        sys_log_debug("%s:crt.key %s", SYS_GET_FUN_NAME, crt->key);

        printf("--------------------erase :%d\n",spi_flash_erase_sector(FLASH_BASE_SECTOR));
        spi_flash_erase_sector(FLASH_BASE_SECTOR + 1);
        rc = spi_flash_write((FLASH_BASE_SECTOR) * 4096, crt, sizeof(sys_flash_iot_crt_t));
        if(rc == ESP_OK) 
        {
            return SYS_OK;
        }
        sys_log_error("%s:flash write fail!", SYS_GET_FUN_NAME);
        return SYS_FAIL;  
    }
    sys_log_error("%s:crt length error!", SYS_GET_FUN_NAME);
	return SYS_FAIL;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int sys_flash_get_flash_crt(sys_flash_iot_crt_t *crt)
{
	esp_err_t rc = ESP_FAIL;

    if(crt == NULL)
	{
        sys_log_error("%s:param NULL!", SYS_GET_FUN_NAME);
		return SYS_FAIL;
	}
    
    rc = spi_flash_read((FLASH_BASE_SECTOR) * 4096, crt, sizeof(sys_flash_iot_crt_t));
    if(rc == ESP_OK)
    {
        sys_log_debug("%s:crt len %d %d %d", SYS_GET_FUN_NAME, strlen(crt->root), strlen(crt->crt), strlen(crt->key));
        sys_log_debug("%s:crt.root %s", SYS_GET_FUN_NAME, crt->root);
        sys_log_debug("%s:crt.crt %s", SYS_GET_FUN_NAME, crt->crt);
        sys_log_debug("%s:crt.key %s", SYS_GET_FUN_NAME, crt->key);

        return SYS_OK;       
    }
    sys_log_error("%s:flash read fail!", SYS_GET_FUN_NAME);
    return SYS_FAIL;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int sys_flash_get_iot_crt(sys_flash_iot_crt_t **crt)
{
    if(*crt != NULL)
	{
        sys_log_error("%s:param no NULL!", SYS_GET_FUN_NAME);
		return SYS_FAIL;
	}
    
    falsh_crt = malloc(sizeof(sys_flash_iot_crt_t));
    if(falsh_crt == NULL)
    {
        sys_log_error("%s:malloc crt param fail!", SYS_GET_FUN_NAME);
		return SYS_FAIL;
    }
    bzero(falsh_crt, sizeof(sys_flash_iot_crt_t));
    if(SYS_OK == sys_flash_get_flash_crt(falsh_crt))
    {
        *crt = falsh_crt;
        if(*crt != NULL)
        {
            return SYS_OK;
        }
        sys_log_error("%s:crt data NULL!", SYS_GET_FUN_NAME);
		return SYS_FAIL;
	}
    sys_log_error("%s:get flash crt fail!", SYS_GET_FUN_NAME);
	return SYS_FAIL;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_flash_free_iot_crt(void)
{
    if(falsh_crt != NULL)
    {
        free(falsh_crt);
    }
    else
    {
        sys_log_error("%s:crt data NULL!", SYS_GET_FUN_NAME);
    }
}