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
        }
        ~OLUP(){}

    public:
        int parse(const char *in);

    public:
        unsigned char cmd(){return oluph_.cmd;}
        unsigned int dev_id(){return oluph_.dev_id;}
        unsigned char dev_type(){return oluph_.dev_type;}

    public:
        OLUPH &oluph(){return oluph_;}
        VersionQuery & version_query(){return version_query_;}
        VersionResp & version_query(){return version_resp_;}
        FirmwareDown & firmware_down(){return firmware_down_;}
        FirmwareResp & firmware_resp(){return firmware_resp_;}

    private:
        OLUPH oluph_;
        VersionQuery version_query_;
        VersionResp version_resp_;
        FirmwareDown firmware_down_;
        FirmwareResp firmware_resp_;
};


#endif

