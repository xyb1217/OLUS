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
    sim_srv.AddListenPort(Cfg::inst().listen_port());
    sim_srv.start();
}


int ImSrv::parse(int fd)
{
    //ÊÕ°ü 
    char *in = NULL;

    //½âÎö
    OLUP olup;
    olup.parse(in);

    Core core;
    core.process();
    
    return 0;
}


