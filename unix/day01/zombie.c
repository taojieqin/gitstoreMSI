#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>


int main(void)
{
	
	pid_t pid;
	signal(SIGCHLD,SIG_IGN);
	printf("before fork pid: %d\n",getpid());

	int num = 10;
	
	pid = fork();
	if(pid >0) //parent
	{
		printf("parent: pid: %d\n", getpid());
		sleep(100);
	}
	else if(pid == 0)
	{
		printf("child:%d, parent:%d\n", getpid(), getppid());
	}
	printf("fork after...\n");
	
	return 0;
}
