#include "core.h"
#include "down_firmware.h"
#include "query_version.h"
#include "report_update_info.h"

#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#include <ccm/im_util.h>
#include <ccm/clog.h>


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
    int ret = readn();
    if (ret != 0){
        if (ret == -1)
    		SVC_LOG((LM_ERROR, "rcv data failed"));
        return ret;
    }
    
    ret = parse();
    if (ret == -1) {
		SVC_LOG((LM_ERROR, "parse failed"));
        return -1;
    }

    ret = response();
    if (ret != 0) {
        if (ret == -1)
    		SVC_LOG((LM_ERROR, "response failed"));
        return ret;
    }
    return 0;
}


//return
//1: client close
//0: ok
//-1: error
int Core::readn()
{
    //parse head: flag1,flag2,len
    char head[3] = {0};
    ssize_t recvn = RecvTimeout(curr_fd_, head, 3, 3);
    if (recvn == 0){
        //client close
		SVC_LOG((LM_ERROR, "client close, recvn:%d", recvn));
        return 1;
    }
    
    if (recvn != 3){
		SVC_LOG((LM_ERROR, "read the first 3 bytes error, recvn:%d", recvn));
        return -1;
    }

    //head[2]:body len
    rcv_len_ = head[2];
    if (rcv_len_ <= 0) return -1;
    
    rcv_len_ += 3;
    if (rcv_buffer_) 
        delete [] rcv_buffer_;
    
    rcv_buffer_ = new char[rcv_len_];
    if (!rcv_buffer_){
		SVC_LOG((LM_ERROR, "new error"));
        return -1;
    }
    memset(rcv_buffer_, 0, rcv_len_);
    memcpy(rcv_buffer_, head, 3);

    recvn = RecvTimeout(curr_fd_, rcv_buffer_+3, head[2], 3);
    if (recvn == 0){
        //client close
		SVC_LOG((LM_ERROR, "client close, recvn:%d", recvn));
        return 1;
    }
    
    if (recvn != head[2]){
		SVC_LOG((LM_ERROR, "read body error"));
        return -1;
    }

    return 0;
}


//1: no update
int Core::parse()
{
    //解析
    int ret = olup_.process(rcv_buffer_, rcv_len_);
    if (ret != 0){
		SVC_LOG((LM_ERROR, "parse on line update protocol failed"));
        return -1;
    }

    if (olup_.cmd() == CMD_VERSION_QUERY){
        ret = version_query();
        if (ret != 0){
            SVC_LOG((LM_ERROR, "query version failed"));
            return -1;
        }
    }
    else if (olup_.cmd() == CMD_FIRMWARE_DOWN){
        ret = firmware_down();
        if (ret != 0){
            SVC_LOG((LM_ERROR, "down firmware failed"));
            return ret;
        }
    }
    else if (olup_.cmd() == CMD_UPDATE_INFO){
        ret = update_info();
        if (ret != 0){
            SVC_LOG((LM_ERROR, "report update info failed"));
            return ret;
        }
    }
    else {
		SVC_LOG((LM_ERROR, "on line update protocol cmd error"));
        return -1;
    }
    
    return 0;
}


int Core::version_query()
{
    QueryVersion query_version;
    int ret = query_version.process(olup_);
    return ret;
}


int Core::firmware_down()
{
    DownFirmware down_firmware;
    int ret = down_firmware.process(olup_);
    return ret;
}


int Core::update_info()
{
    ReportUpdateInfo report_update_info;
    int ret = report_update_info.process(olup_);
    return ret;
}



int Core::response()
{
    
    if (olup_.cmd() == CMD_VERSION_QUERY){
        
        OLUPH *oluph = olup_.oluph();
        oluph->len = 0x09;
        oluph->cmd = 0x90;
        
        oluph->dev_id = htonl(oluph->dev_id);
        ssize_t writen = Writen(curr_fd_, olup_.oluph(), sizeof(OLUPH));
        if (writen != sizeof(OLUPH)){
            SVC_LOG((LM_ERROR, "writen OLUPH failed"));
            return 1;
        }
        
        writen = Writen(curr_fd_, olup_.version_resp(), sizeof(VersionResp));
        if (writen != sizeof(VersionResp)){
            SVC_LOG((LM_ERROR, "writen VersionResp failed"));
            return 1;
        }
        
    }
    else if (olup_.cmd() == CMD_FIRMWARE_DOWN){
        
        OLUPH *oluph = olup_.oluph();
        oluph->len = 0x0D;
        oluph->cmd = 0xA0;
    
        oluph->dev_id = htonl(oluph->dev_id);
        ssize_t writen = Writen(curr_fd_, olup_.oluph(), sizeof(OLUPH));
        if (writen != sizeof(OLUPH)){
            SVC_LOG((LM_ERROR, "writen OLUPH failed"));
            return 1;
        }

        FirmwareResp *firmware_resp = olup_.firmware_resp();
        firmware_resp->down_size = htonl(firmware_resp->down_size);
        writen = Writen(curr_fd_, firmware_resp, sizeof(FirmwareResp));
        if (writen != sizeof(FirmwareResp)){
            SVC_LOG((LM_ERROR, "writen FirmwareResp failed"));
            return 1;
        }
        
        sleep(5);
        //writen = Writen(curr_fd_, olup_.down_info(), olup_.down_size());
        writen = Sendn(curr_fd_, olup_.down_info(), olup_.down_size());
        if (writen != olup_.down_size()){
            SVC_LOG((LM_ERROR, "writen down info failed, writen:%d", writen));
            return 1;
        }

        //下载完成关闭fd
        //return 1;
        return 0;
    }
    else if (olup_.cmd() == CMD_UPDATE_INFO){
        
        OLUPH *oluph = olup_.oluph();
        oluph->len = 0x09;
        oluph->cmd = 0xB0;
        
        oluph->dev_id = htonl(oluph->dev_id);
        ssize_t writen = Writen(curr_fd_, olup_.oluph(), sizeof(OLUPH));
        if (writen != sizeof(OLUPH)){
            SVC_LOG((LM_ERROR, "writen OLUPH failed"));
            return 1;
        }
        
        writen = Writen(curr_fd_, olup_.update_info_resp(), sizeof(UpdateInfo));
        if (writen != sizeof(UpdateInfo)){
            SVC_LOG((LM_ERROR, "writen UpdateInfo failed"));
            return 1;
        }
        
    }
    else {
        SVC_LOG((LM_ERROR, "on line update protocol cmd error"));
        return -1;
    }
    
    return 0;
}



