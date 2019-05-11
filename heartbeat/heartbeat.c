#include <sys/socket.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

static int servfd;
static int nsec;
static int maxnprobes;
static int nprobes;

static void sig_urg(int),sig_alarm(int);

void 
heartbeat_cli(int servfd_arg, int nsec_arg,int maxnprobes_arg)
{
    servfd = servfd_arg;
    if( (nsec = nsec_arg) < 1)
        nsec = 1;
    if( (maxnprobes = maxnprobes_arg) < nsec)
        maxnnprobes = nsec;
    nprobes = 0;

    signal(SIGURG, sig_urg);
    fcntl(servfd, F_SETOWN,getpid());
    alarm(nsec);
}

static void
sig_urg(int signo)
{
    int n;
    char c;
    if( (n = recv(servfd,&c,1,MSG_OOB)) < 0){
        if(errno != EWOULDBLOCK)
            perror("recv error");
    }
    nprobes = 0;
    return;
}

static void
sig_alarm(int signal)
{
    if( ++nprobes > maxnprobes){
        fprintf(stderr,"server is unreachable\n");
        exit(0);
    }
    send(servfd,"1",1,MSGOOB);
    alarm(nsec);
    return;
}


