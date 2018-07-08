#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <signal.h>


#define ERR_EXIT(m) \
	do \
	{\
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)


typedef struct Teacher
{
	char name[64];
	int age;
}Teacher;





void myHandle_forsigaction(int signum, siginfo_t *s_t, void *p)
{
	int myint =0;
	printf("recv signum: %d\n", signum);
	myint = s_t->si_value.sival_int;
	printf("%d %d \n",myint, s_t ->si_int);

}


int main(int argc , char* argv[])
{
	pid_t pid;
	int ret =0;
	struct sigaction act;
	
	act.sa_sigaction = myHandle_forsigaction;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	
	if(sigaction(SIGINT, &act, NULL) < 0)
		ERR_EXIT("sigaction error");

	pid = fork();

	if(pid == -1)
	{
		printf("fork error");
		return 0;
	}
	
	if(pid == 0)
	{
		union sigval mysigval;
		mysigval.sival_int = 111;
		ret = sigqueue(getppid(), SIGINT, mysigval);

		if(ret != 0)
		{
			printf("sigqueue......\n");
			exit(0);
		}
		else
		{
			printf("sigqueue......success\n");
			sleep(2);
		}
	}
	else if(pid >0)
	{
		for(;;)
			pause();
	}
	
	return 0;
}


