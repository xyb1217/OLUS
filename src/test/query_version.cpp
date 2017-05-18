

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
    oluph.cmd = 0x10;
    oluph.dev_id = 0x00000001;
    oluph.dev_id = htonl(oluph.dev_id);
    oluph.dev_type = 0x01;
    //oluph.dev_type = 0x02;
    VersionQuery version_query;
    version_query.check = 0x00;
    version_query.end = 0xEE;
    
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

    writen = Writen(fd, &version_query, sizeof(VersionQuery));
    if (writen != sizeof(VersionQuery)){
        printf("Writen VersionQuery error\n");
        return -1;
    }

    ssize_t recvn = RecvTimeout(fd, &oluph, sizeof(OLUPH), 3);
    if (recvn != sizeof(OLUPH)){
        printf("recvn OLUPH error\n");
        return -1;
    }
    oluph.dev_id = ntohl(oluph.dev_id);
    oluph.pinfo();
    
    VersionResp version_resp;
    recvn = RecvTimeout(fd, &version_resp, sizeof(VersionResp), 3);
    if (recvn != sizeof(VersionResp)){
        printf("recvn VersionResp error, recvn:%d\n", recvn);
        return -1;
    }
    version_resp.pinfo();
    
    return 0;
}



