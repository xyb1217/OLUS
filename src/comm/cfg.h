#ifndef _CFG_H_
#define _CFG_H_

#include <string.h>
#include "cache/down_manage.h"


class Cfg
{
    public:
        static Cfg & inst(){
            static Cfg Cfg;
            return Cfg;
        }
        bool init(const char *cfg_file);

    public:
        int threads(){return threads_;}
        int log_level(){return log_level_;}
        int dev_types(){return dev_types_;}
        int listen_port(){return listen_port_;}
        char *run_id(){return run_id_;}
        char *firmware_path(){return firmware_path_;}
        char *firmware_name(){return firmware_name_;}
        unsigned char firmware_version(){return firmware_version_;}
        char *firmware_file(){return firmware_file_;}

    private:
        Cfg(){
            threads_ = 0;
            log_level_ = 0;
            listen_port_ = -1;
            run_id_ = NULL;
            firmware_path_ = NULL;
            firmware_name_ = NULL;
            firmware_version_ = NULL;
            listen_ip_ = NULL;
            down_info_ = NULL;
        }
        bool load(const char *cfg_file);
        
    private:
        int threads_;
        int log_level_;
        int dev_types_;
        int listen_port_;
        char *listen_ip_;
        char *run_id_;
        char *firmware_path_;
        char *firmware_name_;
        unsigned char firmware_version_;
        char *firmware_file_;

        DownInfo *down_info_;
};


#endif


