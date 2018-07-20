#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>


struct msgbuf{
	long mtype;
	char mtext[1024*10];
};


int main(int argc, char *argv[] )
{
	int msgid;
	int ret = 0;

	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s <type> <bytes> \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int type = atoi(argv[1]);
	int len = atoi(argv[2]);

	msgid = msgget(0x1234, 0666 | IPC_CREAT);
	
	if(msgid == -1)
	{
		if(errno == ENOENT)
			printf("no msg queue\n");
		if(errno == EEXIST)
			printf("msg queue exists\n");
		perror("msgget err");
		return -1;
	}
	printf("msgid: %d\n", msgid);

	struct msgbuf buf;
	
	memset(&buf, 0, sizeof(struct msgbuf));
	buf.mtype = type;
	strcpy(buf.mtext, "11222333aaa");

	ret = msgsnd(msgid, &buf, len, IPC_NOWAIT);
	if(ret < 0 )
	{
		perror("msgsnd err");
		return -1;
	}
	return 0;
}

