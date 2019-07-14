/*
 * @Description: ota header file of system layer
 * @Author: Eric Zhang
 * @Date: 2019-06-23 14:48:21
 * @LastEditTime: 2019-07-04 19:55:16
 * @LastEditors: Please set LastEditors
 */
#ifndef __SYS_OTA_H__
#define __SYS_OTA_H__


/**
 * @description: Get the file size that has been downloaded
 * @param {type} 
 * @return: bytes
 */
int sys_ota_get_file_size(void);

/**
 * @description: delim check,buffer check,further: do an buffer length limited 
 * @param {type} 
 * @return: 
 */
void sys_ota_resp_body_start(void);

/**
 * @description: Upgrade the initialization device for OTA
 * @param {type} 
 * @return:
 *          - -1 fail
 *          - 0  ok 
 */
int sys_ota_upgrade_init(void);

/**
 * @description: 
 * @param {type} 
 * @return: 
 *          - -1 fail
 *          - 0  ok 
 */
int sys_ota_upgrade_loop(int len, char *buf);

#endif
/*************************************************END****************************************************/