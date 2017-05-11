#include "im_srv.h"


int ImSrv::process()
{
    int ret = -1;    
    ret = sim(Cfg::inst().conn(), Cfg::inst().thread());
    return 0;
}


int ImSrv::sim(int conns, int threads)
{
    SimSrv sim_srv(ImSrv::parse, conns, threads);
    sim_srv.SetName("sim_srv");
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
    //ÊÕ°ü 
    char *in = NULL;
    OLUPH oluph;
    //parse head
    ssize_t recvn = Recvn(fd, &oluph, sizeof(OLUPH));
    if (recvn != sizeof(OLUPH)){
        return -1;
    }

    //½âÎö
    OLUP olup;
    olup.parse(in);

    Core core;
    core.process();
    
    return 0;
}


