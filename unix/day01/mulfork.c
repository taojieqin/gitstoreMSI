#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



void TestFun()
{
	printf("fff\n");
}


int main()
{
	int procnum =10;
	int loopnum = 100;
	int i =0;
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
	printf("hello \n");

	return 0;
}
