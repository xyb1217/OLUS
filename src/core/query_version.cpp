#include "query_version.h"


QueryVersion::QueryVersion(){}


QueryVersion::~QueryVersion(){}


int QueryVersion::process(const OLUP &olup)
{   
    if (olup.cmd() != CMD_VERSION_QUERY)
        return -1;

    
    VersionResp *version_resp;
    version_resp = olup.version_resp();
    version_resp->curr_version = Cfg::inst().firmware_version();
    version_resp->check = 0;
    version_resp->end = 0xEE;
        
    return 0;
}

