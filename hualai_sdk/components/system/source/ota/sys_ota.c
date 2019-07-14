/*
 * @Description: 
 * @Author: 
 * @Date: 2019-06-14 16:37:30
 * @LastEditTime: 2019-07-04 19:53:25
 */
#include "sys_base.h"

#include "esp_system.h"
#include "esp_ota_ops.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"


/**
 * @description:  
 */
char resp_body_start = 0;
int all_file_length = 0;
int binary_file_length = 0;

const esp_partition_t *update_partition = NULL;
const esp_partition_t *configured = NULL;
const esp_partition_t *running = NULL;
esp_ota_handle_t update_handle = 0;


/**
 * @description:  
 */
int sys_ota_read_until(char *buffer, char delim, int len);
int sys_ota_read_https_header(char text[], int total_len, esp_ota_handle_t update_handle);
int sys_ota_upgrade_check(void);


/**
 * @description:  
 * @param {type} 
 * @return: 
 */
int sys_ota_get_file_size(void)
{
	return binary_file_length;
}

/**
 * @description:  Resolving protocol header
 * @param {type} 
 * @return: 
 */
void sys_ota_resp_body_start(void)
{
	resp_body_start = 0;
}

/**
 * @description: delim check,buffer check,further: do an buffer length limited 
 * @param {type} 
 * @return: 
 */
int sys_ota_read_until(char *buffer, char delim, int len)
{
    int i = 0;

    while (buffer[i] != delim && i < len) ++i;
    return i + 1;
}

/**
 * @description: return true if packet including \r\n\r\n that means http packet header finished,
 *              start to receive packet body,otherwise return FALSE
 * @param {type} 
 * @return: 
 */
int sys_ota_read_https_header(char text[], int total_len, esp_ota_handle_t update_handle)
{
    int i = 0, i_read_len = 0;

	if(all_file_length == 0) //Only once
	{
		char *length = strstr(text, "Content-Length:");
		if(length != NULL)
		{
			sys_log_debug("%s:all file length %s", SYS_GET_FUN_NAME, length);

			char *data1 = strstr(length, " ");
			char *data2 = strstr(data1, "\r");
			if((data1 != NULL) && (data2 != NULL))
			{
				data1 ++;
				char data[8];
				int data_len = strlen(data1) - strlen(data2);
				memcpy(data, data1, data_len);
				data[data_len] = '\0';
				all_file_length = (int)atoi(data);

                sys_log_info("%s:all_file_length %d", SYS_GET_FUN_NAME, all_file_length);
			}
		}
	}

    while (text[i] != 0 && i < total_len)
    {
        i_read_len = sys_ota_read_until(&text[i], '\n', total_len);
        // if we resolve \r\n line,we think packet header is finished
        if (i_read_len == 2)
        {
            int i_write_len = total_len - (i + 2);
            esp_err_t err = esp_ota_write(update_handle, (const void *)&(text[i + 2]), i_write_len);
            if (err != ESP_OK) 
            {
                sys_log_error("%s:esp_ota_write fail! 0x%x", SYS_GET_FUN_NAME, err)
                return 0;
            } 
            else 
            {
            	sys_log_info("esp_ota_write header %d bytes ok",i_write_len);
                binary_file_length += i_write_len;
            }
            return 1;//ok
        }
        i += i_read_len;
    }
    return 0;
}

/**
 * @description:  
 * @param {type} 
 * @return: 
 */
int sys_ota_upgrade_init(void)
{
    esp_err_t err = ESP_FAIL;

    update_handle = 0;
    update_partition = NULL;
    configured = esp_ota_get_boot_partition();
    running = esp_ota_get_running_partition();
    if(configured != running) 
    {
        sys_log_error("Configured OTA boot partition at offset 0x%08x, but running from offset 0x%08x",
                 configured->address, running->address);
        sys_log_error("(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
    }
    sys_log_info("Running partition type %d subtype %d (offset 0x%08x)",
             running->type, running->subtype, running->address);

    update_partition = esp_ota_get_next_update_partition(NULL);
    sys_log_info("Writing to partition subtype %d at offset 0x%x",
             update_partition->subtype, update_partition->address);
   
    err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle);
    if(err != ESP_OK) 
    {
        sys_log_error("esp_ota_begin failed (%s)", esp_err_to_name(err));
        return -1;
    }
    sys_log_info("esp_ota_begin succeeded");

	all_file_length = 0;
	binary_file_length = 0 ;
	
    resp_body_start = 0;
    
    return 0;
}

/**
 * @description:  
 * @param {type} 
 * @return: 
 */
int sys_ota_upgrade_check(void)
{
    sys_log_info("connection closed, all packets received");
		
    if(esp_ota_end(update_handle) != ESP_OK)
    {
        sys_log_error("esp_ota_end fail!");
        return -1;
    }
    if (esp_ota_set_boot_partition(update_partition) != ESP_OK)
    {
        sys_log_error("esp_ota_set_boot_partition fail!");
        return -1;
    }
    sys_log_info("prepare to restart system!");
    esp_restart();
    return 0;
}

/**
 * @description:  
 * @param {type} 
 * @return: 
 */
int sys_ota_upgrade_loop(int len, char *buf)
{
	esp_err_t err = ESP_FAIL;

	if(len < 0)//receive error
	{ 
		return -1;
	}
	else if(len > 0 && !resp_body_start)//deal with response header
	{ 
		resp_body_start = sys_ota_read_https_header(buf, len, update_handle);
	}
	else if (len > 0 && resp_body_start)//deal with response body
	{ 
		if(binary_file_length < all_file_length)//No acceptance of greater than total length
		{
			err = esp_ota_write(update_handle, (const void *)buf, len);
			if(err != ESP_OK) 
            {
                sys_log_error("%s: esp_ota_write fail! 0x%x", SYS_GET_FUN_NAME, err);
				return -1;
			}
			binary_file_length += len;//Update downloaded file length
			sys_log_info("%d", binary_file_length);
			
			if(all_file_length == binary_file_length)
			{
				binary_file_length = 0;

				return sys_ota_upgrade_check();
			}
		}
		else
		{
			sys_log_error("connection closed, not all packets received!");
		}
	}
	else if(len == 0) //packet over
	{  
		sys_log_info("the packets received");
		sys_log_debug("%s:binary_file_length %d", SYS_GET_FUN_NAME, binary_file_length);
		
		if(binary_file_length < all_file_length)
		{
			return -1;
		}

		if(all_file_length == binary_file_length)
		{
			binary_file_length = 0;

			return sys_ota_upgrade_check();
		}
	}
	else
	{
		sys_log_error("unexpected recv result");
	}
	return 0;
}
