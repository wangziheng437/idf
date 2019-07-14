/*
 * @Description: flash header file of system layer
 * @Author: Eric Zhang
 * @Date: 2019-06-23 14:48:21
 * @LastEditTime: 2019-07-14 15:49:03
 * @LastEditors: Please set LastEditors
 */
#ifndef __SYS_FLASH_H__
#define __SYS_FLASH_H__

/**
 * @description: 
 */
typedef struct{
    char root[2000];
	char crt[2000];
    char key[2000];
}sys_flash_iot_crt_t;

/**
 * @description: Initialise flash
 * @param {type} 
 * @return: 
 */
int sys_flash_int(void);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int sys_flash_set_iot_crt(sys_flash_iot_crt_t *crt);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int sys_flash_get_iot_crt(sys_flash_iot_crt_t **crt);

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
void sys_flash_free_iot_crt(void);


#endif
/*************************************************END****************************************************/