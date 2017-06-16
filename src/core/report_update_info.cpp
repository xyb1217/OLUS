#include "report_update_info.h"
#include "comm/cfg.h"

#include <stdio.h>
#include <ccm/clog.h>

#define SUCCESS_UPDATE_DEV_INFO "success_update_dev_info"

ReportUpdateInfo::ReportUpdateInfo(){}


ReportUpdateInfo::~ReportUpdateInfo(){}


int ReportUpdateInfo::process(OLUP &olup)
{   
    if (olup.cmd() != CMD_UPDATE_INFO){
        SVC_LOG((LM_ERROR, "down firmware protocol cmd error"));
        return -1;
    }

    pthread_mutex_lock (&mutex_write);
    int ret = save_update_info(olup);
    pthread_mutex_unlock (&mutex_write);
    if (ret != 0)
        return -1;
    
    UpdateInfoResp *update_info_resp;
    update_info_resp = olup.update_info_resp();
    update_info_resp->status = 0x11;
    update_info_resp->check = 0;
    update_info_resp->end = 0xEE;


    return 0;
}


int ReportUpdateInfo::save_update_info(OLUP &olup)
{
    DownInfo *down_info;
    OLUPH *oluph = olup.oluph();
    bool bret = DownManage::inst().find(oluph->dev_type, &down_info);
    if (!bret){
        SVC_LOG((LM_ERROR, "find down info error"));
        return -1;
    }

    time_t curr_time = time(NULL);
    tm *now = localtime(&curr_time);
    char time_string[20] = {0};
	snprintf(time_string, sizeof(time_string), "%04d-%02d-%02d %02d:%02d:%02d", 
							now->tm_year+1900, now->tm_mon+1, now->tm_mday, 
							now->tm_hour, now->tm_min, now->tm_sec);

    char file_name[128] = {0};
    snprintf(file_name, sizeof(file_name), "%s%s_%0x", 
                        down_info->firmware_path, 
                        SUCCESS_UPDATE_DEV_INFO,  
                        down_info->dev_type);
    
    FILE *file = fopen(file_name, "a+");
    if (!file) {
        SVC_LOG((LM_ERROR, "open success dev file<%s> failed", 
                            file_name));
        return -1;
    }

    UpdateInfo *update_info = olup.update_info();
    char buffer[1024] = {0};
    snprintf(buffer, sizeof(buffer), 
                "%s,%0x,%0x,%s,%0x", 
                time_string, oluph->dev_id, oluph->dev_type, 
                update_info->dev_num, down_info->firmware_version);
    fprintf(file, "%s\n", buffer);
    
    fclose(file); 
    return 0;
}


