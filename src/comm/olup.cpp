#include "olup.h"
#include <ccm/clog.h>


int OLUP::process(const char *in, int len)
{
    //if ((len != LEN_VERSION_QUERY) && (len != LEN_FIRMWARE_DOWN)){
    if ((!in) || (len <= 0)){
		SVC_LOG((LM_ERROR, "param len error"));
        return -1;
    }
    
    memcpy(&oluph_, in, sizeof(OLUPH));
    oluph_.dev_id = ntohl(oluph_.dev_id);
    oluph_.pinfo();

    if ((oluph_.flag1 != 0x5A) && (oluph_.flag1 != 0xA5)){
        SVC_LOG((LM_ERROR, "protocol start flag error"));
        return -1;
    }
    
    if (oluph_.cmd == CMD_VERSION_QUERY){
        if (oluph_.len != LEN_VERSION_QUERY-3){
            SVC_LOG((LM_ERROR, "version query protocol len error"));
            return -1;
        }
        memcpy(&version_query_, in+sizeof(OLUPH), sizeof(VersionQuery));
        version_query_.pinfo();
    }
    else if (oluph_.cmd == CMD_FIRMWARE_DOWN){
        if (oluph_.len != LEN_FIRMWARE_DOWN-3){
            SVC_LOG((LM_ERROR, "firmware down protocol len error"));
            return -1;
        }
        memcpy(&firmware_down_, in+sizeof(OLUPH), sizeof(FirmwareDown));
        firmware_down_.pinfo();
    }
    else if (oluph_.cmd == CMD_UPDATE_INFO){
        int len = sizeof(OLUPH);
        update_info_.dev_num_len = in[len];
        len = sizeof(OLUPH)+1;
        memcpy(update_info_.dev_num, in+len, update_info_.dev_num_len);
        len = sizeof(OLUPH)+1+update_info_.dev_num_len;
        update_info_.check = in[len];
        len = sizeof(OLUPH)+1+update_info_.dev_num_len+1;
        update_info_.end = in[len];
        
        update_info_.pinfo();
    }
    else {
		SVC_LOG((LM_ERROR, "protocol cmd error"));
        return -1;
    }
    
    return 0;
}



