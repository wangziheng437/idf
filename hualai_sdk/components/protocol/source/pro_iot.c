/*
 * @Description: protocol of iot receive
 * @Author: 
 * @Date: 2019-06-17 18:26:05
 * @LastEditTime: 2019-07-08 03:35:10
 */

#include "cJSON.h"

#include "pro_base.h"
#include "pro_iot.h"

/**
 * @description: 
 */


/**
 * @description: 
 */
int pro_iot_get_timestamp(char *buf, int out_len, char *out);


/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int pro_iot_get_timestamp(char *buf, int out_len, char *out)
{
	if(buf == NULL)
	{
		pro_log_error("%s:param NULL!\n", PRO_GET_FUN_NAME);
		return PRO_CODE_PARAM_NULL;
	}

	char *temp_ts = strstr(buf, "\"Ts\":");
	if(temp_ts == NULL)
	{
		temp_ts = strstr(buf, "\"ts\":");    //Compatible with different forms
		if(temp_ts == NULL)
		{
			temp_ts = strstr(buf, "\"TS\":");
		}
	}

	if(temp_ts != NULL)
	{
		strncpy(out, temp_ts + 5, 13);
		out[13] = '\0';
		if(strlen(out) < out_len)
		{
			return PRO_CODE_SUCCECSS;
		}
		pro_log_error("%s:insufficient string length!", PRO_GET_FUN_NAME);
		return PRO_CODE_LENGTH_ERROR;
	}
	pro_log_error("%s:analysis timestamp fail!\n", PRO_GET_FUN_NAME);
	return PRO_CODE_VALUE_NULL;
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int pro_iot_receive_data(char *buf, pro_iot_rec_t *out)
{
	if((buf == NULL) || (out == NULL))
	{
		pro_log_error("%s:param NULL!\n", PRO_GET_FUN_NAME);
		return PRO_CODE_PARAM_NULL;
	}

	char *json = strchr(buf, '{');
	if(json == NULL)
	{
		pro_log_error("%s:no json found!\n", PRO_GET_FUN_NAME);
		return PRO_CODE_TYPE_ERROR;
	}
	pro_log_debug("%s:sub json = %s %d", PRO_GET_FUN_NAME, json, strlen(json));

	cJSON *json_root = cJSON_Parse(json);
	if(json_root == NULL)
	{
		pro_log_error("%s:analysis json fail!\n", PRO_GET_FUN_NAME);
		return PRO_CODE_VALUE_NULL;
	}
	
	char json_ts[15] = {};
	int time_stamp = pro_iot_get_timestamp(json, sizeof(json_ts), json_ts);
	cJSON *json_action = cJSON_GetObjectItem(json_root, "action");
	cJSON *json_params = cJSON_GetObjectItem(json_root, "params");
	cJSON *json_guid   = cJSON_GetObjectItem(json_root, "guid");	
	if((json_action == NULL) || (json_params == NULL) || (json_guid == NULL) || (time_stamp != PRO_CODE_SUCCECSS))
	{
		cJSON_Delete(json_root);
		json_root = NULL;
		pro_log_error("%s:analysis json item fail!", PRO_GET_FUN_NAME);
		return PRO_CODE_VALUE_NULL;
	}

	char *temp_params = cJSON_PrintUnformatted(json_params);
	if(temp_params == NULL)
	{
		cJSON_Delete(json_root);
		json_root = NULL;
		pro_log_error("%s:analysis params item fail!", PRO_GET_FUN_NAME);
		return PRO_CODE_VALUE_NULL;
	}

	if((json_action->valuestring == NULL) && (json_guid->valuestring == NULL))
	{
		cJSON_Delete(json_root);
		free(temp_params);
		json_root = NULL;
		temp_params = NULL;
		pro_log_error("%s:analysis params value fail!", PRO_GET_FUN_NAME);
		return PRO_CODE_VALUE_NULL;
	}

	int action_len = strlen(json_action->valuestring)+ 1;
	int params_len = strlen(temp_params)+ 1;
	int guid_len   = strlen(json_guid->valuestring) + 1;
	int ts_len     = strlen(json_ts) + 1;
	if((action_len < sizeof(out->action)) && (params_len < sizeof(out->params)) 
		&& (guid_len < sizeof(out->guid)) && (ts_len < sizeof(out->ts)))
	{
		memcpy(out->action, json_action->valuestring, action_len);
		memcpy(out->params, temp_params, params_len);
		memcpy(out->guid, json_guid->valuestring, guid_len);
		memcpy(out->ts, json_ts, ts_len);

		pro_log_debug("%s:action = %s %d", PRO_GET_FUN_NAME, out->action, action_len);
		pro_log_debug("%s:params = %s %d", PRO_GET_FUN_NAME, out->params, params_len);
		pro_log_debug("%s:guid = %s %d", PRO_GET_FUN_NAME, out->guid, guid_len);
		pro_log_debug("%s:ts = %s %d", PRO_GET_FUN_NAME, out->ts, ts_len);
		
		cJSON_Delete(json_root);
		free(temp_params);
		json_root = NULL;
		temp_params = NULL;
		return PRO_CODE_SUCCECSS;
	}
	pro_log_error("%s:params value too long!", PRO_GET_FUN_NAME);
	cJSON_Delete(json_root);
	free(temp_params);
	json_root = NULL;
	temp_params = NULL;
	return PRO_CODE_LENGTH_ERROR;	
}

/**
 * @description: 
 * @param {type} 
 * @return: 
 */
int pro_iot_params_upgrade(char *param, pro_iot_upgrade_t *out)
{
	if((param == NULL) || (out == NULL))
	{
		pro_log_error("%s:param NULL!\n", PRO_GET_FUN_NAME);
		return PRO_CODE_PARAM_NULL;
	}
	
	cJSON *json_root = cJSON_Parse(param);
	if(json_root == NULL)
	{
		pro_log_error("%s:analysis json fail!\n", PRO_GET_FUN_NAME);
		return PRO_CODE_VALUE_NULL;
	}

	cJSON *json_url	  = cJSON_GetObjectItem(json_root, "url");
	cJSON *json_md5	  = cJSON_GetObjectItem(json_root, "md5");
	cJSON *json_model = cJSON_GetObjectItem(json_root, "model");
	if((json_url == NULL) || (json_md5 == NULL) || (json_model == NULL))
	{
		cJSON_Delete(json_root);
		json_root = NULL;
		pro_log_error("%s:analysis json item fail!", PRO_GET_FUN_NAME);
		return PRO_CODE_VALUE_NULL;
	}
	
	if((json_url->valuestring == NULL) || (json_md5->valuestring == NULL) || (json_model->valuestring == NULL))
	{
		cJSON_Delete(json_root);
		json_root = NULL;
		pro_log_error("%s:analysis params value fail!", PRO_GET_FUN_NAME);
		return PRO_CODE_VALUE_NULL;
	}
	
	int url_len   = strlen(json_url->valuestring) + 1;
	int md5_len   = strlen(json_md5->valuestring) + 1;
	int model_len = strlen(json_model->valuestring) + 1;
	if((url_len < sizeof(out->url)) && (md5_len < sizeof(out->md5)) && (model_len < sizeof(out->model)))
	{
		memcpy(out->url, json_url->valuestring, url_len);
		memcpy(out->md5, json_md5->valuestring, md5_len);
		memcpy(out->model, json_model->valuestring, model_len);
		
		pro_log_debug("%s:url = %s %d", PRO_GET_FUN_NAME, out->url, url_len);
		pro_log_debug("%s:md5 = %s %d", PRO_GET_FUN_NAME, out->md5, md5_len);
		pro_log_debug("%s:model = %s %d", PRO_GET_FUN_NAME, out->model, model_len);
		
		cJSON_Delete(json_root);
		json_root = NULL;
		return PRO_CODE_SUCCECSS;
	}
	pro_log_error("%s:params value too long!", PRO_GET_FUN_NAME);
	cJSON_Delete(json_root);
	json_root = NULL;
	return PRO_CODE_LENGTH_ERROR;
}

/**
 * @Descripttion: 
 * @param {type} 
 * @return: 
 */
int pro_iot_send_ack(char *guid, char result, char *out, int out_len)
{
	if((guid == NULL) || (out == NULL) || (out_len <= 0))
	{
		pro_log_error("%s:param NULL!\n", PRO_GET_FUN_NAME);
		return PRO_CODE_PARAM_NULL;
	}

	int len = 0;
	len += snprintf(out + len, out_len - len, "{\"guid\":\"%s\",", guid);
	len += snprintf(out + len, out_len - len, "\"result\":\"%d\"}", result);
	if(len < out_len)
	{
		pro_log_debug("%s:ack = %s %d", PRO_GET_FUN_NAME, out, len);
		return len;
	}
	pro_log_error("%s:insufficient string length!", PRO_GET_FUN_NAME);
	return PRO_CODE_LENGTH_ERROR;
}

/**
 * @Descripttion: 
 * @param {type} 
 * @return: 
 */
int pro_iot_send_data(pro_iot_send_t *in, char *out, int out_len)
{
	if((in == NULL) || (out == NULL) || (out_len <= 0))
	{
		pro_log_error("%s:param NULL!\n", PRO_GET_FUN_NAME);
		return PRO_CODE_PARAM_NULL;
	}

	if((strlen(in->mac) < sizeof(in->mac)) && (strlen(in->data)< sizeof(in->data)) 
		&& (strlen(in->model) < sizeof(in->model)))
	{
		int len= 0;
		len += snprintf(out + len, out_len - len, "{\"mac\":\"%s\",", in->mac);
		len += snprintf(out + len, out_len - len, "\"model\":\"%s\",", in->model);
		len += snprintf(out + len, out_len - len, "\"data\":\"%s\",", in->data);
		len += snprintf(out + len, out_len - len, "\"ts\":%s}", in->ts);

		if(len < out_len)
		{
			pro_log_debug("%s:send = %s %d", PRO_GET_FUN_NAME, out, len);
			return len;
		}
		pro_log_error("%s:insufficient string length!", PRO_GET_FUN_NAME);
		return PRO_CODE_LENGTH_ERROR;
	}
	pro_log_error("%s:params value overflow!", PRO_GET_FUN_NAME);
	return PRO_CODE_LENGTH_ERROR;
}

/**
 * @Descripttion: 
 * @param {type} 
 * @return: 
 */
int pro_iot_topic_choice(int topic, char *client_id, char *out, int out_len)
{
	int len= 0;
	
	if((client_id == NULL) || (out == NULL))
	{
		pro_log_error("%s:param NULL!\n", PRO_GET_FUN_NAME);
		return PRO_CODE_PARAM_NULL;
	}

	switch(topic)
	{
		case 0:
			len = snprintf(out, out_len, "hl/devices/%s/cmd",client_id);
			break;
		case 1:
			len = snprintf(out, out_len, "hl/devices/%s/cmd/ack",client_id);
			break;
		case 2:
			len = snprintf(out, out_len, "hl/v1/devices/%s/property/report",client_id);
			break;
		default:
			pro_log_error("%s:no item found %d!", PRO_GET_FUN_NAME, topic);
			break;
	}

	if(len < out_len)
	{
		pro_log_debug("%s:topic = %s %d", PRO_GET_FUN_NAME, out, len);
		return len;
	} 
	pro_log_error("%s:insufficient string length!", PRO_GET_FUN_NAME);
	return PRO_CODE_LENGTH_ERROR;
}

