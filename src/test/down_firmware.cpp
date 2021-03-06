

#include "comm/comm.h"
#include <ccm/im_util.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    if (argc < 3){
        printf("param error, argc:%d\n", argc);
        return -1;
    }
    
    OLUPH oluph;
    oluph.flag1 = 0x5A;
    oluph.flag2 = 0xA5;
    oluph.len = 0x08;
    oluph.cmd = 0x20;
    oluph.dev_id = 0x00000001;
    oluph.dev_id = htonl(oluph.dev_id);
    oluph.dev_type = 0x01;
    //oluph.dev_type = 0x02;
    FirmwareDown firmware_down;
    firmware_down.check = 0x00;
    firmware_down.end = 0xEE;
    
    int fd = Socket();
    if (fd <= 0){
        printf("socket error\n");
        return -1;
    }
    printf("ip:%s, port:%d\n", argv[1], atoi(argv[2]));
    
    int ret = Connect(fd, argv[1], atoi(argv[2]));
    if (ret != 0){
        printf("Connect error\n");
        Close(fd);
        return -1;
    }

    ssize_t writen = Writen(fd, &oluph, sizeof(OLUPH));
    if (writen != sizeof(OLUPH)){
        printf("Writen OLUPH error\n");
        Close(fd);
        return -1;
    }

    writen = Writen(fd, &firmware_down, sizeof(FirmwareDown));
    if (writen != sizeof(FirmwareDown)){
        printf("Writen FirmwareDown error\n");
        Close(fd);
        return -1;
    }

    ssize_t recvn = RecvTimeout(fd, &oluph, sizeof(OLUPH), 3);
    if (recvn != sizeof(OLUPH)){
        printf("recvn OLUPH error\n");
        Close(fd);
        return -1;
    }
    oluph.dev_id = ntohl(oluph.dev_id);
    oluph.pinfo();
    
    FirmwareResp firmware_resp;
    recvn = RecvTimeout(fd, &firmware_resp, sizeof(FirmwareResp), 3);
    if (recvn != sizeof(FirmwareResp)){
        printf("recvn FirmwareResp error\n");
        Close(fd);
        return -1;
    }
    
    firmware_resp.down_size = ntohl(firmware_resp.down_size);
    firmware_resp.pinfo();

    char *down_info = new char [firmware_resp.down_size];
    if (!down_info){
        printf("new down buffer error, size:%d\n", firmware_resp.down_size);
        Close(fd);
        return -1;
    }

    /*int recv_total = 0;
    int recv_pre = 1000;
    int recv_len = firmware_resp.down_size;
    while (recv_len > 0){
        if (recv_len >= recv_pre){
            recvn = RecvTimeout(fd, down_info+recv_total, recv_pre, 10);
            if (recvn != recv_pre){
                printf("recvn down info error1, recvn:%d, recv_pre:%d\n", recvn, recv_pre);
                if (down_info) delete [] down_info;
                Close(fd);
                return -1;
            }
        }
        else {
            recvn = RecvTimeout(fd, down_info+recv_total, recv_len, 10);
            if (recvn != recv_pre){
                printf("recvn down info error1, recvn:%d, recv_len:%d\n", recvn, recv_len);
                if (down_info) delete [] down_info;
                Close(fd);
                return -1;
            }
        }
        recv_total += recv_pre;
        recv_len -= recv_pre;
    }*/
    
    recvn = RecvTimeout(fd, down_info, firmware_resp.down_size, 10);
    if (recvn != firmware_resp.down_size){
        printf("recvn down info error, recvn:%d, size:%d\n", 
                recvn, firmware_resp.down_size);
        if (down_info) delete [] down_info;
        Close(fd);
        return -1;
    }
    
    printf("down info: %s\n", down_info);
    if (down_info) delete [] down_info;
    
    Close(fd);
    return 0;
}

