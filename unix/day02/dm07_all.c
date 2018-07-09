#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


#define ERR_EXIT(m)\
	do \
	{  \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)


void handler(int num)
{
	if(num == SIGUSR1)
	{
		sigset_t bset;
		sigemptyset(&bset);
		sigaddset(&bset, SIGINT);
		sigaddset(&bset, SIGRTMIN);
		sigprocmask(SIG_UNBLOCK, &bset, NULL);
	}
	else if(num == SIGINT || num == SIGRTMIN)
		printf("recv sig num: %d \n", num);
}


int main()
{
	pid_t pid;
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	

	if( sigaction(SIGINT, &act, NULL) < 0)
		ERR_EXIT("sigaction SIGINT");
	
	if( sigaction(SIGRTMIN, &act, NULL) < 0)
		ERR_EXIT("sigaction, SIGRTMIN");

	if( sigaction(SIGUSR1, &act, NULL) < 0)
		ERR_EXIT("sigaction SIGUSR1");

	// block SIGINT SIGRTMIN
	sigset_t bset;
	sigemptyset(&bset);
	sigaddset(&bset,SIGINT);
	sigaddset(&bset, SIGRTMIN);
	sigprocmask(SIG_BLOCK,&bset, NULL );

	pid = fork();
	if( pid == -1)
		ERR_EXIT("fork err");
	
	if(pid == 0)
	{
		union sigval v;
		int ret;
		v.sival_int = 201;
		int i;
		for(i=0; i< 3; i++)
		{
			ret = sigqueue(getppid(), SIGINT, v);
			if( ret != 0)
			{
				printf("send SIGINT failure: %d", errno);
				exit(0);
			}
			else
			{
				printf("send SIGINT success \n");
			}
		}
		v.sival_int = 301;
		for(i=0; i< 3; i++)
		{
			ret = sigqueue(getppid(), SIGRTMIN, v);
			if( ret != 0)
			{
				printf("send SIGRTMIN failure: %d", errno);
				exit(0);
			}
			else
			{
				printf("send SIGRTMIN success\n");
			}
		}
		kill(getppid(), SIGUSR1);
	}

	while(1)
	{
		pause();
	}
		

	return 0;
}

