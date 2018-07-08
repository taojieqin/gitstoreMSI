#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


void handle(int num)
{
	printf("recv a sig:%d\n", num);
}


void  handler2(int num, siginfo_t *info, void *p)
{
	printf("recv sig: %d \n", num);

}

int main(int argc, char argv[])
{
	struct sigaction act;
	
	// alternative 
	//act.sa_handler = handle;
	act.sa_sigaction = handler2;

	sigaction(SIGINT, &act,NULL);

	for(;;)
	{
		sleep(1);
	}

	return 0;
}
