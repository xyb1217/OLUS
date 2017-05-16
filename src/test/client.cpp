#include "comm/comm.h"


int main(char *argv[], int argc)
{
    OLUPH oluph;
    VersionQuery version_query;
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
    
    VersionResp version_resp;
    recvn = RecvTimeout(fd, &version_resp, sizeof(VersionResp), 3);
    if (recvn != sizeof(VersionResp)){
        printf("recvn VersionResp error\n");
        return -1;
    }
    
    return 0;
}


