#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void hander(int num)
{
	printf("recv num:%d\n", num);
	if(num == SIGQUIT)
		exit(0);
}

void main()
{
	__sighander oldHandle;


	oldHandle = signal(SIGINT, hander);
	//signal(SIGINT, oldHandle);
	signal(SIGQUIT, hander);

	while(1)
		pause();

	printf("main begin \n");

}
