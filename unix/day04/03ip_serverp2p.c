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
	printf("recv num:%d\n",num);
}


int main()
{
	int sockfd = 0;
	
	signal(SIGUSR1, handle);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		EXIT("socket error\n");	
	int optval =1;
	if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0 )
		EXIT("set socket opt error");

	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8001);
	srvaddr.sin_addr.s_addr = inet_addr("192.168.50.128");
	//srvaddr.sin_addr.s_addr = INADDR_ANY; //Bind any addr in local
	
	if( bind(sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr) )  < 0 )
	{
		perror("bind error\n");
		exit(0);
	}
	
	int backlog = SOMAXCONN;

	/*
		if listen is used, sockfd can only wait for connnections and 
					  cannot ask for connections.
	*/
	if(listen(sockfd,backlog) <0)
	{
		perror("listen error\n");
		exit(0);
	}

	/*
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	*/
	struct sockaddr_in peeraddr; // tcp/ip struct
	socklen_t peerlen =  sizeof(peeraddr);
	

	int conn=0;	
	conn = accept(sockfd, (struct sockaddr *)&peeraddr, &peerlen);
	if(conn <0)
		EXIT("accept error\n");
		
	printf("peeraddr:%s\n perrport:%d\n", inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port) );
	pid_t pid = fork();
		
	char recvbuf[1024] ={0};
	char sendbuf[1024] = {0};
	if(pid == 0)
	{
		while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
		{
			write(conn, sendbuf,sizeof(sendbuf));
			memset(sendbuf,0, sizeof(sendbuf));
		}
	}
	else if(pid >0)
	{
		while(1)
		{
			int  ret = read(conn, recvbuf, sizeof(recvbuf));
			if(ret == 0)
			{	// return 0 if client is closed
				printf("client %d has been closed\n", conn);
				break;
				
			}
			else if(ret <0)
			{
				printf("read error\n");
				exit(0);
			}
			fputs(recvbuf, stdout);
			memset(recvbuf, 0, sizeof(recvbuf));
		}
		close(conn);
		kill(pid, SIGUSR1);
		sleep(1);
	}
	else
	{	
		close(conn);
		close(sockfd);
		EXIT("fork error\n");
	}

	close(sockfd);

	return 0;
}



