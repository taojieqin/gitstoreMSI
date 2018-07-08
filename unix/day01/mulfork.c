#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

void TestFun()
{
	printf("child\n");
}


int main()
{
	int procnum =10;
	int loopnum = 100;
	int i =0;
	int ret;
	pid_t pid;

	for(i=0; i< procnum; i++)
	{
		pid = fork();
		if(pid == 0)
		{
			TestFun();
			exit(0);
		}
	}
	

// ?????????????????????????????????
	while(1)
	{
		ret = wait(NULL);
		if(ret == -1)
		{
			if(errno == EINTR)
				continue;
			break;
		}
	}
	printf("parent quit \n");

	return 0;
}

