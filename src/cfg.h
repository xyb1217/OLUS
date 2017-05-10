#ifndef _CFG_H_
#define _CFG_H_

class Cfg
{
    public:
        static Cfg & inst(){
            static Cfg Cfg;
            return Cfg;
        }
        bool init(const char *cfg_file);

    public:
        int thread(){return threads_;}
        int conn(){return conns_;}
        int log_level(){return log_level_;}
        int listen_port(){return listen_port_;}
        int run_id(){return run_id_;}
        int firmware_path(){return firmware_path_;}
        int firmware_name(){return firmware_name_;}
        int firmware_version(){return firmware_version;}

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
        }
        bool load(const char *cfg_file);
        
    private:
        int threads_;
        int conns_;
        int log_level_;
        int listen_port_;
        char *listen_ip_;
        char *run_id_;
        char *firmware_path_;
        char *firmware_name_;
        char *firmware_version_;
        char *listen_ip_;
};


#endif


