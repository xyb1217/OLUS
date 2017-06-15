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

    int ret = save_update_info(olup);
    if (ret != 0)
        return -1;
    
    UpdateInfoResp *update_info_resp;
    update_info_resp = olup.update_info_resp();
    update_info_resp->verify = 0x11;
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
    
    FILE *file = fopen("SUCCESS_UPDATE_DEV_INFO", "w+");
    if (!file) {
        SVC_LOG((LM_ERROR, "open down file<%s> failed", 
                            down_info->firmware_file));
        return -1;
    }

    UpdateInfo *update_info = olup.update_info();
    char buffer[1024] = {0};
    snprintf(buffer, sizeof(buffer), 
                "%s_%s_%s_%d", 
                oluph->dev_id, oluph->dev_type, 
                update_info->dev_num, down_info->firmware_version);
    fprintf(file, "%s\n", buffer);
    
    fclose(file); 
    return 0;
}


