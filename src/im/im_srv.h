#ifndef _IM_SRV_H_
#define _IM_SRV_H_

#define MAX_CONNS 1024

class ImSrv
{
    public:
        int process();
        
    private:
        static int parse(int fd);
        
        
};

#endif


