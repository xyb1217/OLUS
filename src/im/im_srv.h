#ifndef _IM_SRV_H_
#define _IM_SRV_H_


class ImSrv
{
    public:
        int process();
        
    private:
        
        int sim(int conns, int threads);
        static int parse(int fd);
        
        
};

#endif


