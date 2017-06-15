#ifndef _OLUP_H_
#define _OLUP_H_

#include "comm.h"


class OLUP
{
    public:
        OLUP(){
            memset(&oluph_, 0, sizeof(OLUPH));
            memset(&version_query_, 0, sizeof(VersionQuery));
            memset(&version_resp_, 0, sizeof(VersionResp));
            memset(&firmware_down_, 0, sizeof(FirmwareDown));
            memset(&firmware_resp_, 0, sizeof(FirmwareResp));
            down_info_ = NULL;
            down_size_ = 0;
        }
        
        ~OLUP(){
            if (down_info_)
                delete [] down_info_;
        }

    public:
        int process(const char *in, int len);

    public:
        unsigned char cmd(){return oluph_.cmd;}
        unsigned int len(){return oluph_.len+3;}
        unsigned int dev_id(){return oluph_.dev_id;}
        unsigned char dev_type(){return oluph_.dev_type;}

    public:
        OLUPH *oluph(){return &oluph_;}
        VersionQuery *version_query(){return &version_query_;}
        VersionResp *version_resp(){return &version_resp_;}
        FirmwareDown *firmware_down(){return &firmware_down_;}
        FirmwareResp *firmware_resp(){return &firmware_resp_;}
        UpdateInfo *update_info(){return &update_info_;}
        UpdateInfoResp *update_info_resp(){return &update_info_resp_;}
        char *down_info(){return down_info_;}
        int down_size(){return down_size_;}
        void set_down_info(char *down_info, int down_size){
            down_info_ = down_info;
            down_size_ = down_size;
            return ;
        }


    private:
        OLUPH oluph_;
        VersionQuery version_query_;
        VersionResp version_resp_;
        FirmwareDown firmware_down_;
        FirmwareResp firmware_resp_;
        UpdateInfo update_info_;
        UpdateInfoResp update_info_resp_;
        char *down_info_;
        int down_size_;
};


#endif

