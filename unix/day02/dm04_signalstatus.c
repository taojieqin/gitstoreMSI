#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>


#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}while(0)



void handle(int num)
{
	if (num == SIGINT)
	{
		printf("recv signal SIGINT");
	}
	else if( num == SIGQUIT)
	{
		sigset_t uset;
		sigemptyset(&uset);
		sigaddset(&uset, SIGINT);
		sigprocmask(SIG_UNBLOCK, &uset, NULL);
	}

}



void printsigset(sigset_t *set)
{
	int i;
	for(i=1; i<NSIG; ++i)
		if(sigismember(set, i))
			putchar('1');
		else
			putchar('0');
	printf("\n");
}




int main(int argc, char *argv[])
{
	sigset_t pset;
	sigset_t bset;

	sigemptyset(&bset);
	sigaddset(&bset, SIGINT);
	
	if(signal(SIGINT, handle) == SIG_ERR)
		ERR_EXIT("signal error");
	if(signal(SIGQUIT, handle) == SIG_ERR)
		ERR_EXIT("signal error");
	
	sigprocmask(SIG_BLOCK, &bset, NULL);   // ctrl+c is blocked
	
	for(;;)
	{
		sigpending(&pset);
		printsigset(&pset);
		sleep(1);
	}

	return 0;
}
