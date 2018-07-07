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
	
	int fd;
	pid_t pid;
	signal(SIGCHLD,SIG_IGN);
	printf("before fork pid: %d\n",getpid());

	int num = 10;
	
	fd = open("11.txt", O_CREAT, 0666 ); //S_IRUSR | S_IWUSR);
	if(fd == -1)
	{
		perror("open error \n");
		return 0;
	}	
	pid = fork();
	if(pid == -1)
	{
		printf("pid <0, err.\n");
		return -1;
	}
	if(pid >0) //parent
	{
		printf("parent: pid: %d\n", getpid());
		write(fd, "parent",6);
		close(fd);
	}
	else if(pid == 0)
	{
		printf("child:%d, parent:%d\n", getpid(), getppid());
		write(fd,"child",5);
		close(fd);
	}
	printf("fork after...\n");
	
	return 0;
}
