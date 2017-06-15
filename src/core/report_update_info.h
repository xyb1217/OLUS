#ifndef _REPORT_UPDATE_INFO_H_
#define _REPORT_UPDATE_INFO_H_

#include "comm/olup.h"

class ReportUpdateInfo
{
    public:
        ReportUpdateInfo();
        ~ReportUpdateInfo();

    public:
        int process(OLUP &olup);

    private:
        int save_update_info(OLUP &olup);
        
};

#endif

