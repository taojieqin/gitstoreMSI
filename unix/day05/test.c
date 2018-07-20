#include <stdio.h>
#include <stdlib.h>


void fun(int ** p)
{
	int b =100;
	*p = &b;

}

void main()
{

	int a =10; 
	int *q = &a;
	//	printf("value: %d \n",*q);
	printf("value: %d, addr: %p\n",*q, q);

	fun(&q);
	
	printf("value: %d, addr: %p\n",*q, q);
}
