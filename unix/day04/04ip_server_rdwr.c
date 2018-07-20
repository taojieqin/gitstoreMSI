#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)
	

struct packet
{
	int len;
	char buf[1024];
};


ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nread;
	char *bufp = (char *)buf;

	while(nleft > 0)
	{
		if( (nread = read(fd, bufp, nleft)) < 0)
		{
			if(errno == EINTR)
				continue;
			return -1;
		}
		else if( nread == 0)
			return count - nleft;
		
		bufp += nread;
		nleft -= nread;
	}
	return count;

}


ssize_t written(int fd, const void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nwritten;
	char *bufp = (char *)buf;
	
	while(nleft > 0)
	{
		if((nwritten = write(fd, bufp, nleft)) < 0)
		{
			if(errno == EINTR)
				continue;
			return -1;
		}
		else if(nwritten ==0)
			continue;
		bufp += nwritten;	
		nleft -= nwritten;
	}
	return count;
}


void do_service(int conn)
{
	struct packet recvbuf;
	int n;
	while(1)
	{
		memset(&recvbuf, 0, sizeof(recvbuf));
		int ret = readn(conn, &recvbuf.len, 4);
		if( ret == -1)
			ERR_EXIT("read error\n");
		else if(ret < 4)
		{
			printf("client close\n");
			break;
		}
		n = ntohl(recvbuf.len);
		ret = readn(conn, recvbuf.buf, n);
		if(ret == -1)
			ERR_EXIT("read error\n");
		else if(ret < n)
		{
			printf("client close\n");
			break;
		}
		fputs(recvbuf.buf, stdout);
		written(conn, &recvbuf, 4+n);
	}
}




int main(void)
{
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
		if((conn = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0)
			ERR_EXIT("accept error\n");
		printf("ip = %s port = %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
		
		pid = fork();
		
		if(pid == -1)
			ERR_EXIT("fork error\n");
		if(pid == 0)
		{
			close(listenfd);
			do_service(conn);
			exit(EXIT_SUCCESS);
		}
		else
			close(conn);
	}

	return 0;
}

