#include "down_firmware.h"
#include "comm/cfg.h"

#include <stdio.h>
#include <ccm/clog.h>

DownFirmware::DownFirmware(){}


DownFirmware::~DownFirmware(){}


int DownFirmware::process(OLUP &olup)
{   
    if (olup.cmd() != CMD_FIRMWARE_DOWN){
        SVC_LOG((LM_ERROR, "down firmware protocol cmd error"));
        return -1;
    }
    
    FirmwareResp *firmware_resp;
    firmware_resp = olup.firmware_resp();
    firmware_resp->verify = 0x01;
    firmware_resp->check = 0;
    firmware_resp->end = 0xEE;

    int down_size = get_down_info(olup);
    if (down_size <= 0){
        firmware_resp->verify = 0x00;
        SVC_LOG((LM_ERROR, "get down info failed"));
        return 1;
    }
    firmware_resp->down_size = down_size;
                
    return 0;
}


int DownFirmware::get_down_info(OLUP &olup)
{
    DownInfo *down_info;
    OLUPH *oluph = olup.oluph();
    bool bret = DownManage::inst().find(oluph->dev_type, &down_info);
    if (!bret){
        SVC_LOG((LM_ERROR, "find down info error"));
        return -1;
    }
    
    FILE *file = fopen(down_info->firmware_file, "r");
    if (!file) {
        SVC_LOG((LM_ERROR, "open down file<%s> failed", 
                            down_info->firmware_file));
        return -1;
    }
    
    char *down_buffer = NULL; 
    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    down_buffer = new char[len+1]; 
    if (!down_buffer) {
        SVC_LOG((LM_ERROR, "new down buffer error, size:%d", len+1));
        return -1;
    }

    rewind(file);
    fread(down_buffer, 1, len, file);
    down_buffer[len] = 0;
    fclose(file); 

    olup.set_down_info(down_buffer, len);
    
    return len;
}


