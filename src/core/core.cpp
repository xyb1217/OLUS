
Core::Core()
{
    curr_fd_ = -1;
    rcv_buffer_ = NULL;
    rcv_len_ = 0;
}


Core::~Core()
{
    curr_fd_ = -1;
    rcv_len_ = 0;
    if (rcv_buffer_)
        delete [] rcv_buffer_;
    rcv_buffer_ = NULL;
}


int Core::process(int fd)
{
    curr_fd_ = fd;
    int ret = parse(fd);
    if (ret != 0) {
        return -1;
    }

    ret = response();
    if (ret != 0) {
        return -1;
    }
    return 0;
}



int Core::parse()
{
    int ret = recvn();
    if (ret != 0){
        return -1;
    }

    if (olup_.cmd() == CMD_VERSION_QUERY){
        ret = version_query();
        if (ret != 0){
            return -1;
        }
    }
    else if (olup_.cmd() == CMD_FIRMWARE_DOWN){
        ret = firmware_down();
        if (ret != 0){
            return -1;
        }
    }
    else {
        return -1;
    }
    
    return 0;
}


int Core::recvn()
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

    //½âÎö
    int ret = olup_.process(rcv_buffer_, rcv_len_);
    if (ret != 0){
        return -1;
    }

    return 0;
}


int Core::version_query()
{
    QueryVersion query_version;
    int ret = query_version.process(olup_);
    if (ret != 0){
        return -1;
    }
    return 0;
}


int Core::firmware_down()
{
    DownFirmware down_firmware;
    int ret = down_firmware.process(olup_);
    if (ret != 0){
        return -1;
    }
    return 0;
}


int Core::response()
{
    if (olup_.cmd() == CMD_VERSION_QUERY){
        ssize_t writen = Writen(curr_fd_, olup_.oluph(), sizeof(OLUPH));
        if (writen != sizeof(OLUPH))
            return -1;
        
        writen = Writen(curr_fd_, olup_.version_resp(), sizeof(VersionResp));
        if (writen != sizeof(VersionResp))
            return -1;
        
    }
    else if (olup_.cmd() == CMD_FIRMWARE_DOWN){
        ssize_t writen = Writen(curr_fd_, olup_.oluph(), sizeof(OLUPH));
        if (writen != sizeof(OLUPH))
            return -1;
        
        writen = Writen(curr_fd_, olup_.firmware_resp(), sizeof(FirmwareResp));
        if (writen != sizeof(FirmwareResp))
            return -1;
        
        writen = Writen(curr_fd_, olup_.down_info(), olup_.down_size());
        if (writen != olup_.down_size())
            return -1;
    }
    else {
        return -1;
    }
    
    return 0;
}



