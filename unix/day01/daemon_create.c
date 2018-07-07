#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


int setup_daemon(int nochdir, int noclose);


int main(int argc, char *argv[])
{
	setup_daemon(1,1);
	
	// daemon(1,1); // 0 root dirctory  ; 1 current working directory
	
	printf("test...\n");

	for(;;);

	return 0;
}



int setup_daemon(int nochdir, int noclose)
{
	pid_t pid;
	pid = fork();
	
	if(pid == -1)
	{
		perror("fork error");
	}
	
	if(pid >0)
		exit(EXIT_SUCCESS);

	setsid();
	
	if(nochdir ==0)
		chdir("/");

	if(noclose == 0)
	{
		int i;
		for(i=0;i<3; i++)
			close(i);
		open("/dev/null", O_RDWR);
		dup(0);
		dup(0);
	}
	return 0;
}

