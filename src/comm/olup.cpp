#include "olup.h"


int OLUP::process(const char *in, int len)
{
    memcpy(&oluph_, in, sizeof(OLUPH));
    if (oluph_.cmd == CMD_VERSION_QUERY){
        if (oluph_.len != LEN_VERSION_QUERY-3){
            return -1;
        }
        memcpy(&version_query_, in+sizeof(OLUPH), sizeof(VersionQuery));
    }
    else if (oluph_.cmd == CMD_FIRMWARE_DOWN){
        if (oluph_.len != LEN_FIRMWARE_DOWN-3){
            return -1;
        }
        memcpy(&firmware_down_, in+sizeof(OLUPH), sizeof(FirmwareDown));
    }
    else {
        return -1;
    }
    
    return 0;
}



