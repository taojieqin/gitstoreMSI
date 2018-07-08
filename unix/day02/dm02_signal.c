#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>



void myhandle(int num)
{
	if( num == SIGINT)
		printf("recv signal SIGINT \n");
	else if (num == SIGUSR1)
		printf("recv signal SIGUSR1 \n" );
	else
		printf("recv signal id num: %d\n", num);
}


int main(void)
{
	pid_t pid;

	printf("main....begin \n");
	if(signal(SIGINT,myhandle) == SIG_ERR)
	{
		perror("func signal err \n");
		return 0;
	}
	if(signal(SIGUSR1, myhandle) == SIG_ERR)
	{
		perror("func signal err \n");
		return 0;
	}
	
	pid = fork();
	
	if(pid == -1)
	{
		printf("fork err....\n");
		return  0;
	}

	if(pid == 0)
	{
		pid = getppid();
		kill(0, SIGUSR1);
		exit(0);
	}
	int n = 13;
	do
	{
		printf("parent sleep start \n");
		n = sleep(n);
		printf("parent wake up\n" );
	}while(n >0);

}

