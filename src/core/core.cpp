
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


int Core::process(char *rcv_buffer, int len, char **out_buffer, int *out_len)
{
    curr_fd_ = fd;
    int ret = parse(rcv_buffer, len);
    if (ret != 0) {
        return -1;
    }

    ret = response();
    if (ret != 0) {
        return -1;
    }
    return 0;
}



int Core::parse(const char *rcv_buffer, int len)
{
    //����
    int ret = olup_.process(rcv_buffer, len);
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



