
#include "sim_srv.h"

SimSrv::SimSrv(){}
SimSrv::~SimSrv(){}


int SimSrv::process(int fd)
{
    Core core;
    core.process(fd);
    return 0;
}




