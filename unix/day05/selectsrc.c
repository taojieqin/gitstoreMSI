#include "sckutil.h"


	

void handle_sigchld(int sig)
{

	int pid =0;
	printf("recv signum:%d \n", sig);
	while( (pid = waitpid(-1, NULL, WNOHANG) ) > 0)
	{
		printf("quit child pid: %d\n", pid);
		fflush(stdout);
	}
}



int main(void)
{
	// signal(SIGCHLD, SIG_IGN);
	signal(SIGCHLD, handle_sigchld);
	int listenfd;
	if((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket error\n");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8001);
	servaddr.sin_addr.s_addr = inet_addr("192.168.50.128");

	int on =1;
	if( setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 )
		ERR_EXIT("setsockopt error\n");
	
	if( bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind error\n");
	
	if(listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen error\n");

	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int conn;

	pid_t pid;	

	while(1)
	{	
		int ret = 0;
		int wait_seconds = 5;
		
		if((conn = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0)
			ERR_EXIT("accept error\n");
		printf("ip = %s port = %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
		
		pid = fork();
		
		if(pid == -1)
			ERR_EXIT("fork error\n");
		if(pid == 0)
		{
			char recvbuf[1024];
			int recvbuflen = 1024;
			int datalen;
			close(listenfd);
			
			while(1)
			{
				memset(recvbuf, 0, sizeof(recvbuf) );
				ret = read_timeout(conn, wait_seconds);
				if(ret == 0)
				{
					datalen = readline(conn, recvbuf, sizeof(recvbuf));
					if(datalen < 0)
						ERR_EXIT("readline error\n");
					else if( datalen == 0)
					{
						printf("client close\n");
						close(conn);
						ERR_EXIT("readline return 0\n");
					}
				}
				else if(ret == -1 && errno == ETIMEDOUT)
				{
					printf("read time out\n");
					continue;
				}
				else if(ret < 0)
					ERR_EXIT("select error\n");
				fputs(recvbuf, stdout);
				
				int wtlen = 0;
				ret = write_timeout(conn, wait_seconds);
				if(ret == 0)
				{
					wtlen = writen(conn, recvbuf, datalen);
					printf("writen: %d \n" , wtlen);
				}	
				else if(ret == -1 && errno == ETIMEDOUT )
				{
					printf("write time out\n");
					continue;
				}
				else if(ret < 0)
					ERR_EXIT("select error\n");
			}

		}
		else if(pid >0)
			close(conn);
	}

	return 0;
}

