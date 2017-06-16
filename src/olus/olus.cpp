
#include "comm/cfg.h"
#include "comm/comm.h"
#include "im/im_srv.h"

#include <ccm/clog.h>
#include <ccm/ccm.h>

#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



#define CFG_FILE "../cfg/olus.xml"
#define LOG_PATH "../log/"

ImSrv *im_srv = NULL;


void handle_pipe(int sig)
{
    PRINTF("pipe error: %d", sig);
    return ;
}


void pipe_sig_handle()
{
    struct sigaction action;
    action.sa_handler = handle_pipe;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGPIPE, &action, NULL);
    return ;
}


void init()
{
    sigset_t ipc_mask;
    sigemptyset(&ipc_mask);
    sigaddset(&ipc_mask, SIGALRM);
    sigaddset(&ipc_mask, SIGPIPE);
    sigaddset(&ipc_mask, SIGCHLD);
    pthread_sigmask(SIG_BLOCK,&ipc_mask,NULL);
    pipe_sig_handle();

    CLog *pLog = CLog::inst();
    if (!pLog->init(LOG_PATH, "olus")){
        PRINTF("init log error, exit");
        sleep(3);
        exit(1);
    }

    pLog->set_loglevel(Cfg::inst().log_level());
    if (pLog->open(1) == -1){
        pLog->finalize();
        PRINTF("start log error, exit");
        sleep(3);
        exit(1);
    }

    bool bret = Cfg::inst().init(CFG_FILE);
    if (!bret){
        PRINTF("init cfg error, exit");
        sleep(3);
        exit(1);
    }
    pthread_mutex_init (&mutex_write, NULL);

    SVC_LOG((LM_INFO, "init success"));
    return ;
}


void im()
{
    im_srv = new ImSrv;
    if (!im_srv){
        PRINTF("new ImSrv error, exit");
        sleep(3);
		exit(1);
    }
    
    int ret = im_srv->process();
    if (ret != 0){
        PRINTF("init im srv error, exit");
        sleep(3);
		exit(1);
    }
    return ;
}


void daemon() 
{
    int pid, fd;
    if ((pid = fork()) == -1) exit(1);
    if (pid != 0) exit(0);
    if (setsid() == -1) exit(1);
    if ((pid = fork()) == -1) exit(1);
    if (pid != 0) exit(0);
    //for (int i = 0; i < NOFILE; i++)
    //	close(i);
    int fdtablesize = getdtablesize();
    for (fd = 3; fd < fdtablesize; fd++)
    {
        close(fd);
    }
    //if (chdir("/") == -1) exit(1);
    //if ((fd = open("/dev/null", O_RDWR)) == -1) exit(1);
    //dup2(fd, STDIN_FILENO);
    //dup2(fd, STDOUT_FILENO);
    //dup2(fd, STDERR_FILENO);
    //close(fd);
    if ((int)umask(0) == -1) exit(1);
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) exit(1);
    return ;
}



int main()
{
    daemon();
    init();
    im();

    while (1)
        sleep(60);
    
    return 0;
};


