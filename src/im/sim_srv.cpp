
#include "sim_srv.h"

SimSrv::SimSrv(){}
SimSrv::~SimSrv()
{
    if (rcv_buffer_)
        delete [] rcv_buffer_;
    rcv_buffer_ = NULL;
}


int SimSrv::process(int fd)
{
    //parse head: flag1,flag2,len
    char head[3] = {0};
    ssize_t recvn = Recvn(curr_fd_, head, 3);
    if (recvn != 3){
        return -1;
    }

    //head[2]:body len
    rcv_len_ = head[2]+3;
    if (rcv_buffer_) 
        delete [] rcv_buffer_;
    
    rcv_buffer_ = new char[rcv_len_];
    if (!rcv_buffer_){
        return -1;
    }
    memset(rcv_buffer_, 0, rcv_len_);
    memcpy(rcv_buffer_, head, 3);

    recvn = Recvn(curr_fd_, rcv_buffer_+3, head[2]);
    if (recvn != head[2]){
        return -1;
    }
    
    Core core;
    core.process(rcv_buffer_, rcv_len_, );
    return 0;
}




