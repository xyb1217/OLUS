#ifndef _SIM_SRV_H_
#define _SIM_SRV_H_


class SimSrv
{
    public:
        SimSrv();
        ~SimSrv();

        int process(int fd);

    private:
        char *rcv_buffer_;
        int rcv_len_;

        
};

#endif
