#include "query_version.h"
#include "comm/cfg.h"
#include <ccm/clog.h>


QueryVersion::QueryVersion(){}


QueryVersion::~QueryVersion(){}


int QueryVersion::process(OLUP &olup)
{ 
    if (olup.cmd() != CMD_VERSION_QUERY){
        SVC_LOG((LM_ERROR, "query version protocol cmd error"));
        return -1;
    }
    
    VersionResp *version_resp;
    version_resp = olup.version_resp();
    
    //version_resp->curr_version = Cfg::inst().firmware_version();
    DownInfo *down_info;
    OLUPH *oluph = olup.oluph();
    bool bret = DownManage::inst().find(oluph->dev_type, &down_info);
    if (!bret){
        SVC_LOG((LM_ERROR, "find version error"));
        return -1;
    }
    version_resp->curr_version = down_info->firmware_version;
    version_resp->check = 0;
    version_resp->end = 0xEE;
        
    return 0;
}

