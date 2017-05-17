
#ifndef _COMM_H_
#define _COMM_H_

#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#define CMD_VERSION_QUERY 0x10
#define CMD_FIRMWARE_DOWN 0x20
#define CMD_VERSION_RESP  0x90
#define CMD_FIRMWARE_RESP 0xA0

#define LEN_VERSION_QUERY  11
#define LEN_FIRMWARE_DOWN  11

#pragma pack (1)

struct OLUPH 
{
    unsigned char flag1;
    unsigned char flag2;
    unsigned char len;
    unsigned char cmd;
    unsigned int dev_id;
    unsigned char dev_type;

    OLUPH(){
        memset(this, 0, sizeof(OLUPH));
    }

    void pinfo(){
        printf("\n***********OLUPH**********\n");
        printf("flag1:%0X\n", flag1);
        printf("flag2:%0X\n", flag2);
        printf("len:%0X\n", len);
        printf("cmd:%0X\n", cmd);
        printf("dev_id:%0X\n", dev_id);
        printf("dev_type:%0X\n", dev_type);
        printf("************OLUPH*********\n");
    }
};


struct VersionQuery
{ 
    unsigned char check;
    unsigned char end;

    VersionQuery(){
        memset(this, 0, sizeof(VersionQuery));
    }

    void pinfo(){
        printf("\n***********VersionQuery**********\n");
        printf("check:%0X\n", check);
        printf("end:%0X\n", end);
        printf("***********VersionQuery**********\n");
    }
};


struct VersionResp
{
    unsigned char curr_version;
    unsigned char check;
    unsigned char end;

    VersionResp(){
        memset(this, 0, sizeof(VersionResp));
    }

    void pinfo(){
        printf("\n**********VersionResp***********\n");
        printf("curr_version:%0X\n", curr_version);
        printf("check:%0X\n", check);
        printf("end:%0X\n", end);
        printf("***********VersionResp**********\n");
    }
};


struct FirmwareDown
{
    unsigned char check;
    unsigned char end;

    FirmwareDown(){
        memset(this, 0, sizeof(FirmwareDown));
    }

    void pinfo(){
        printf("\n***********FirmwareDown**********\n");
        printf("check:%0X\n", check);
        printf("end:%0X\n", end);
        printf("***********FirmwareDown**********\n");
    }
};


struct FirmwareResp
{
    unsigned char verify;
    unsigned int down_size;
    unsigned char check;
    unsigned char end;

    FirmwareResp(){
        memset(this, 0, sizeof(FirmwareResp));
    }

    void pinfo(){
        printf("\n***********FirmwareResp**********\n");
        printf("verify:%0X\n", verify);
        printf("down_size:%d\n", down_size);
        printf("check:%0X\n", check);
        printf("end:%0X\n", end);
        printf("***********FirmwareResp**********\n");
    }
};
#pragma pack ()


extern void n2h_oluph(OLUPH & oluph);
extern void h2n_oluph(OLUPH & oluph);
extern void n2h_firmware_resp(FirmwareResp & firmware_resp);
extern void h2n_firmware_resp(FirmwareResp & firmware_resp);



#endif


