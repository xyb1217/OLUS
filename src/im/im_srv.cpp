#include "im_srv.h"


int ImSrv::process()
{
    if (Cfg::inst().im_mode()== 0){
        SimSrv sim_srv(ImSrv::parse, 
                        Cfg::inst().conn(), 
                        Cfg::inst().thread());
        sim_srv.SetName("sim_srv");
        sim_srv.AddListenPort(Cfg::inst().listen_port());
        bool bret = sim_srv.start();
        if (!bret){
            SVC_LOG((LM_INFO, "start sim_srv error"));
            return -1;
        }
        SVC_LOG((LM_INFO, "start sim_srv success"));
    }
    else {
        return -1;
    }
    return 0;
}


int ImSrv::sim(int fd)
{
    SimSrv sim_srv;
    int ret = sim_srv.process(fd);
    if (ret != 0)
        return -1;
    return 0;
}



