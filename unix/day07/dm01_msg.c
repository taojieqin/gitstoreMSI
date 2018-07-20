#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>






int maini01()
{
	int msgid;
	msgid = msgget(0x1234, 0666 | IPC_CREAT);
	if(msgid == -1)
	{	
		if(errno == ENOENT)
			printf("no massage in the queue\n");
		perror("msgget err\n");
		return -1;
	}	
	printf("msgid:%d\n", msgid);
	printf("creat msg_ipc\n");

	return 0;
}


// if exist perror file exist; else create a new file
int main02()
{
	int msgid;
	msgid = msgget(0x1234, 0666 | IPC_CREAT | IPC_EXCL);
	if(msgid == -1)
	{	
		if(errno == ENOENT)
			printf("no massage in the queue\n");
		if(errno == EEXIST)
			printf("massage ipc exists\n");
		perror("msgget err");
		return -1;
	}	
	printf("msgid:%d\n", msgid);
	printf("creat msg_ipc\n");

	return 0;
}




// IPC_PRIVATE  msg ipc created can only be used by family 
// IPC_PRIVATE  IPC_CREAT IPC_EXCL exist, msgid is different each time to this function
int main()
{
	int msgid;
	msgid = msgget(IPC_PRIVATE, 0666 );
	if(msgid == -1)
	{	
		if(errno == ENOENT)
			printf("no massage in the queue\n");
		if(errno == EEXIST)
			printf("massage ipc exists\n");
		perror("msgget err");
		return -1;
	}	
	printf("msgid:%d\n", msgid);
	printf("creat msg_ipc\n");

	return 0;
}
