#include <unistd.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
//#include "sckutil.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "commsocket.h"

void handle(int signum)
{
	int pid = 0;
	printf("recv signum:%d \n", signum);
	
	//避免僵尸进程
	while ((pid = waitpid(-1, NULL, WNOHANG) ) > 0)
	{
		printf("退出子进程pid%d \n", pid);
		fflush(stdout);
	} 
}


int main()
{	
	int 		ret = 0;
	void 		*handle = NULL;
	//void 		handle = NULL;
	int 		connfd;
	
		
	 unsigned char *data = (unsigned char *)"aaaaaafffffffffffssssss";
	 int 		datalen = 10;
	 
	 unsigned char out[1024];
	 int outlen = 1024;
	//客户端环境初始化
	//int sckCliet_init(void **handle, char *ip, int port, int contime, int sendtime, int revtime);
	ret = sckCliet_init(&handle, 15, 5, 5, 10);
	
	ret = sckCliet_getconn(handle, "192.168.50.128", 8001, &connfd);
	//客户端发送报文
	ret = sckClient_send(handle,  connfd, data, datalen);
	
	
	//客户端端接受报文
	ret = sckClient_rev(handle, connfd, out, &outlen);
	
	
	// 客户端环境释放 
	ret = sckClient_destroy(handle);

}


