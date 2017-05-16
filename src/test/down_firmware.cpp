

#include "comm/comm.h"
#include <ccm/im_util.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    OLUPH oluph;
    oluph.flag1 = 0x5A;
    oluph.flag2 = 0xA5;
    oluph.len = 0x08;
    oluph.cmd = 0x20;
    oluph.dev_id = 0x00000001;
    oluph.dev_type = 0x01;
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
        return -1;
    }

    ssize_t writen = Writen(fd, &oluph, sizeof(OLUPH));
    if (writen != sizeof(OLUPH)){
        printf("Writen OLUPH error\n");
        return -1;
    }

    writen = Writen(fd, &firmware_down, sizeof(FirmwareDown));
    if (writen != sizeof(FirmwareDown)){
        printf("Writen FirmwareDown error\n");
        return -1;
    }

    ssize_t recvn = RecvTimeout(fd, &oluph, sizeof(OLUPH), 3);
    if (recvn != sizeof(OLUPH)){
        printf("recvn OLUPH error\n");
        return -1;
    }
    oluph.pinfo();
    
    FirmwareResp firmware_resp;
    recvn = RecvTimeout(fd, &firmware_resp, sizeof(FirmwareResp), 3);
    if (recvn != sizeof(FirmwareResp)){
        printf("recvn FirmwareResp error\n");
        return -1;
    }
    firmware_resp.pinfo();

    char *down_info = new char [firmware_resp.down_size];
    if (!down_info){
        printf("new down buffer error, size:%d\n", firmware_resp.down_size);
        return -1;
    }

    recvn = RecvTimeout(fd, down_info, firmware_resp.down_size, 3);
    if (recvn != firmware_resp.down_size){
        printf("recvn down info error\n");
        if (down_info) delete [] down_info;
        return -1;
    }
    printf("down info: %s\n", down_info);
    if (down_info) delete [] down_info;
    
    return 0;
}

