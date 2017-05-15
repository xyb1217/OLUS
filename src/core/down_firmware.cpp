#include "down_firmware.h"
#include "comm/cfg.h"

#include <stdio.h>

DownFirmware::DownFirmware(){}


DownFirmware::~DownFirmware(){}


int DownFirmware::process(OLUP &olup)
{   
    if (olup.cmd() != CMD_FIRMWARE_DOWN)
        return -1;

    int ret = down_info(olup);
    if (ret <= 0)
        return -1;

    FirmwareResp *firmware_resp;
    firmware_resp = olup.firmware_resp();
    firmware_resp->verify = 0x01;
    firmware_resp->down_size = 0;
    firmware_resp->check = 0;
    firmware_resp->end = 0xEE;
            
    return 0;
}


int DownFirmware::down_info(OLUP &olup)
{
    FILE *file = fopen(Cfg::inst().firmware_file(), "r");
    if (!file) return -1;
    
    char *down_info = NULL; 
    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    down_info = new char[len+1]; 
    if (!down_info) return -1;

    rewind(file);
    fread(down_info, 1, len, file);
    down_info[len] = 0;
    fclose(file); 

    olup.set_down_info(down_info, len);
    
    return len;
}


