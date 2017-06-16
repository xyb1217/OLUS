

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
    oluph.cmd = 0x30;
    oluph.dev_id = 0x00000001;
    oluph.dev_id = htonl(oluph.dev_id);
    oluph.dev_type = 0x01;
    //oluph.dev_type = 0x02;
    
    UpdateInfo update_info;
    strcpy((char *)update_info.dev_num, "test_num_12345");
    update_info.dev_num_len = strlen((char *)update_info.dev_num);
    update_info.check = 0x00;
    update_info.end = 0xEE;

    oluph.len = 9+update_info.dev_num_len;
    
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

    unsigned char tmp_buffer[128] = {0};
    int len = 1;
    memcpy(tmp_buffer, (char *)&update_info.dev_num_len, len);
    len = 1;
    memcpy(tmp_buffer+1, update_info.dev_num, update_info.dev_num_len);
    len = 1+update_info.dev_num_len;
    memcpy(tmp_buffer+len, (char *)&update_info.check, 1);
    len = 1+update_info.dev_num_len+1;
    memcpy(tmp_buffer+len, (char *)&update_info.end, 1);
    len = 1+update_info.dev_num_len+1+1;
    
    writen = Writen(fd, tmp_buffer, len);
    if (writen != len){
        printf("Writen UpdateInfo error\n");
        return -1;
    }

    ssize_t recvn = RecvTimeout(fd, &oluph, sizeof(OLUPH), 3);
    if (recvn != sizeof(OLUPH)){
        printf("recvn OLUPH error\n");
        return -1;
    }
    oluph.dev_id = ntohl(oluph.dev_id);
    oluph.pinfo();
    
    UpdateInfoResp update_info_resp;
    recvn = RecvTimeout(fd, &update_info_resp, sizeof(UpdateInfoResp), 3);
    if (recvn != sizeof(UpdateInfoResp)){
        printf("recvn UpdateInfoResp error, recvn:%d\n", recvn);
        return -1;
    }
    update_info_resp.pinfo();
    
    return 0;
}



