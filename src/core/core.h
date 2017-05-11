
#ifndef _CORE_H_
#define _CORE_H_


class Core
{
    public:
        Core();
        ~Core();
        
    public:
        int process(int fd);
        
    private:
        int parse();
        int recvn();
        int version_query();
        int firmware_down();
        int response();

    private:
        int curr_fd_;
        OLUP olup_;
        char *rcv_buffer_;
        int rcv_len_;
        
};

#endif
