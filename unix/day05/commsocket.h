#ifndef _SCK_CLINT_H_
#define _SCK_CLINT_H_


#ifdef __cplusplus
extern 'C'
{
#endif 

// define error type


#define Sck_Ok		0
#define Sck_BaseErr	3000

#define Sck_ErrParam		(Sck_BaseErr + 1)
#define Sck_ErrTimeout		(Sck_BaseErr + 2)
#define Sck_ErrPeerClosed	(Sck_BaseErr + 3)
#define Sck_ErrMalloc		(Sck_BaseErr + 4)



int sckCliet_init(void **handle, int contime, int sendtime, int revtime, int nConNum);
int sckCliet_getconn(void *handle, char *ip, int port, int *connfd);
int sckCliet_closeconn(int connfd);


int sckClient_send(void *handle, int connfd, unsigned char *data, int datalen);

int sckClient_rev(void *handle, int connfd, unsigned char *out, int *outlen);

int sckClient_destroy(void *handle);


#ifdef __cplusplus
}
#endif

#endif
