
#ifndef _CORE_H_
#define _CORE_H_


class Core
{
    public:
        Core();
        ~Core();
        
    public:
        int process(char *rcv_buffer, int len, char **out_buffer, int *out_len);
        
    private:
        int parse(const char *rcv_buffer, int len);
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
