
#ifndef _COMM_H_
#define _COMM_H_


#define CMD_VERSION_QUERY 0x10
#define CMD_FIRMWARE_DOWN 0x20
#define CMD_VERSION_RESP  0x90
#define CMD_FIRMWARE_RESP 0xA0


struct OLUPH 
{
    unsigned char flag1;
    unsigned char flag2;
    unsigned char len;

    OLUPH(){
        memset(this, 0, sizeof(OLUPH));
    }
};


struct VersionQuery
{
    unsigned char cmd;
    unsigned int dev_id;
    unsigned char dev_type;
    unsigned char check;
    unsigned char end;

    VersionQuery(){
        memset(this, 0, sizeof(VersionQuery));
    }
};


struct VersionResp
{
    unsigned char cmd;
    unsigned int dev_id;
    unsigned char dev_type;
    unsigned char curr_version;
    unsigned char check;
    unsigned char end;

    VersionResp(){
        memset(this, 0, sizeof(VersionResp));
    }
};


struct FirmwareDown{
    unsigned char cmd;
    unsigned int dev_id;
    unsigned char dev_type;
    unsigned char check;
    unsigned char end;

    FirmwareDown(){
        memset(this, 0, sizeof(FirmwareDown));
    }
};


struct FirmwareResp
{
    unsigned char cmd;
    unsigned int dev_id;
    unsigned char dev_type;
    unsigned char verify;
    unsigned int down_size;
    unsigned char check;
    unsigned char end;

    FirmwareResp(){
        memset(this, 0, sizeof(FirmwareResp));
    }
};



#endif


