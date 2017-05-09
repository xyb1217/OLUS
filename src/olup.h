#ifndef _OLUP_H_
#define _OLUP_H_

#include "comm.h"


class OLUP
{
    public:
        static OLUP &inst(){
            static OLUP olup;
            return olup;
        }

    private:
        OLUPH oluph_;
        VersionQuery version_query_;
        VersionResp version_resp_;
        FirmwareDown firmware_down_;
        FirmwareResp firmware_resp_;
};


#endif

