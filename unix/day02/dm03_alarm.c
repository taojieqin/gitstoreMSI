#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void myhandle(int num)
{
	printf("recv signal id num: %d \n", num);
	alarm(1);
}



int main(void)
{
	printf("main.... begin\n");

	if(signal(SIGALRM, myhandle)== SIG_ERR)
	{
		perror("func signal err\n");
		return 0;
	}

	alarm(1);
	while(1)
	{
		pause();
		printf("pause return \n");
	}
	
	return 0;
}
