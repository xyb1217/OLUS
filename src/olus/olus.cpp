
#include "comm/cfg.h"
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
	
	bool bret = Cfg::inst().init(CFG_FILE);
	if (!bret){
		PRINTF("init cfg error, exit");
		exit(1);
	}

    CLog *pLog = CLog::inst();
	if (!pLog->init(LOG_PATH, "olus")){
		PRINTF("init log error, exit");
		exit(1);
	}

	pLog->set_loglevel(Cfg::inst().log_level());
	if (pLog->open(1) == -1){
		pLog->finalize();
		PRINTF("start log error, exit");
		exit(1);
	}    
	SVC_LOG((LM_INFO, "init success"));
	return ;
}


void im()
{
    ImSrv im_srv;
    int ret = im_srv.process();
    if (ret != 0){
        PRINTF("init im srv error, exit");
		exit(1);
    }
    return ;
}


int main()
{
    init();
    im();

    while (1)
        sleep(60);
    
    return 0;
};


