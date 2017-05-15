#ifndef _DOWN_FIRMWARE_H_
#define _DOWN_FIRMWARE_H_

#include "comm/olup.h"

class DownFirmware
{
    public:
        DownFirmware();
        ~DownFirmware();

    public:
        int process(OLUP &olup);

    private:
        int down_info(OLUP &olup);
        
};

#endif

