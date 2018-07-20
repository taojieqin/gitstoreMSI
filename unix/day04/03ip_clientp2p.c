#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define EXIT(m) \
	do{\
		perror(m);\
		exit(0);\
	}while(0)


void handle(int num)
{
	printf("recv num:%d\n", num);
	exit(0);
}	



int main()
{
	

	signal(SIGUSR1, handle);
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		EXIT("socket error\n");	
	
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8001);
	addr.sin_addr.s_addr = inet_addr("192.168.50.128");
	//addr.sin_addr.s_addr = INADDR_ANY; //Bind any addr in local
	

	if( connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) <0)
		EXIT("connect error\n");
	
	char recvbuf[1024] ={0};
	char sendbuf[1024] ={0};
	

	pid_t pid = fork();

	if( pid > 0 )
	{
		
		while( fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) 
		{
			write(sockfd, sendbuf, strlen(sendbuf));
			memset(sendbuf,0, sizeof(sendbuf));
		}
	}
	else if(pid == 0)
	{
		while(1)
		{
			int ret =0;
			ret = read(sockfd, recvbuf,sizeof(recvbuf));
			if(ret <= 0)
			{
				printf("read err\n");
				break;
			}
			else
			{
				fputs(recvbuf, stdout);
	
				memset(recvbuf,0, sizeof(recvbuf));
			}
		}
		close(sockfd);
		kill(getppid(), SIGUSR1);
		exit(0);
	}
		
	close(sockfd);

	return 0;
}



