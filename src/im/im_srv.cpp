#include "im_srv.h"
#include "comm/cfg.h"
#include "core/core.h"

#include <ccm/sim_srv.h>
#include <ccm/clog.h>



int ImSrv::process()
{
    SimSrv sim_srv(ImSrv::parse,  MAX_CONNS, 
                    Cfg::inst().threads());
    sim_srv.set_name("sim_srv");
    sim_srv.AddListenPort(Cfg::inst().listen_port());
    bool bret = sim_srv.start();
    if (!bret){
        SVC_LOG((LM_INFO, "start sim_srv error"));
        return -1;
    }
    SVC_LOG((LM_INFO, "start sim_srv success"));
    return 0;
}


int ImSrv::parse(int fd)
{   
    Core core;
    int ret = core.process(fd);
    if (ret != 0)
        return -1;
    return 0;
}



