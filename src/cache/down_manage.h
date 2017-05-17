//brand manage cache
#ifndef _DOWN_MANAGE_H_
#define _DOWN_MANAGE_H_

#include "comm/comm.h"
#include <openssl/lhash.h>

using namespace std;


struct DownInfo
{
    int dev_type;
    char *firmware_path;
    char *firmware_name;
    char *firmware_file;
    int firmware_version;

    DownInfo(){
        memset(this, 0, sizeof(DownInfo));
        dev_type = -1;
        firmware_path = NULL;
        firmware_name = NULL;
        firmware_file = NULL;
        firmware_version = -1;
    }

    void free(){
        if (firmware_path)
            delete [] firmware_path;
        firmware_path = NULL;

        if (firmware_name)
            delete [] firmware_name;
        firmware_name = NULL;

        if (firmware_file)
            delete [] firmware_file;
        firmware_file = NULL;
    }
};


class DownManage
{
    public:
        static DownManage & inst(){
            static DownManage instance;
            return instance;
        }

        bool init(unsigned int max_items);

    public:

        bool insert(DownInfo *down_info);
        bool del(int dev_type);
        bool update(DownInfo *down_info);
        bool find(int dev_type, DownInfo **down_info);

        void clear();
        unsigned int items();

    public:
        static unsigned long dev_hash(const void *buffer);
        static int dev_cmp(const void *buffer1, 
                            const void *buffer2);
    	

    private:
        DownManage();
        DownManage(const DownManage &down_manage);
        DownManage &operator=(const DownManage &down_manage);


    private:
        _LHASH *lhash_dev_type_;
        unsigned int max_items_;
        pthread_mutex_t mutex_;

};

#endif
