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




int main()
{
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		EXIT("socket error\n");	
	/*
	
       int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

           struct sockaddr_in {
               sa_family_t    sin_family; // address family: AF_INET 
               in_port_t      sin_port;   // port in network byte order 
               struct in_addr sin_addr;   // internet address 
		};
          // Internet address. 
           struct in_addr {
               uint32_t       s_addr;     // address in network byte order 
           };
	*/
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8001);
	addr.sin_addr.s_addr = inet_addr("192.168.50.128");
	//addr.sin_addr.s_addr = INADDR_ANY; //Bind any addr in local
	

	if( connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) <0)
		EXIT("connect error\n");
	
	char recvbuf[1024] ={0};
	char sendbuf[1024] ={0};
	while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
	{
		write(sockfd, sendbuf, strlen(sendbuf));
		
		read(sockfd, recvbuf,sizeof(recvbuf));
		
		fputs(recvbuf, stdout);
	
		memset(recvbuf,0, sizeof(recvbuf));
		memset(sendbuf,0, sizeof(sendbuf));
	}
	close(sockfd);

	return 0;
}



