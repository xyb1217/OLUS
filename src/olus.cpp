


#define CFG_FILE "../cfg/olus.xml"
#define LOG_PATH "../log/"

int main()
{
    //cfg
    bool bret = Cfg::inst().init(CFG_FILE);
    if (!bret) return -1;

    //log
    
    //start srv
    ImSrv im_srv;
    im_srv.process();
    
    return 0;
};


