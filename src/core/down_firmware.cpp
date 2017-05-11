#include "down_firmware.h"


DownFirmware::DownFirmware(){}


DownFirmware::~DownFirmware(){}


int DownFirmware::process(const OLUP &olup)
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


int DownFirmware::down_info(const OLUP &olup)
{
    FILE *file = fopen(Cfg::inst().firmware_file(), "r");
    if (!file) return -1;
    
    char *read_buffer = NULL; 
    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    read_buffer = new char[len+1]; 
    if (!read_buffer) return -1;
    
    olup.down_info() = read_buffer;
    rewind(file);
    fread(read_buffer, 1, len, file);
    read_buffer[len] = 0;
    fclose(file); 

    olup.firmware_resp().down_size = len;
    return len;
}


