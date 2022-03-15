#include<iostream>
#include <stdlib.h>
#include "test.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

using namespace std;

#define BUF_SIZE 512
char buf[BUF_SIZE];
int main() {
	freopen("res.txt","r",stdout);
	printf("first line\n");
	int fd_read = open(fifo_stov,O_RDONLY);
	int fd_write = open(fifo_vtos,O_WRONLY);
while(true){
	read(fd_read,buf,2);
	int data;

	sscanf(buf,"%d",&data);
	
	sprintf(buf,"%d",data+data);
	write(fd_write,buf,2);
	printf("Vmodel:Waiting for message\n");
	}
	return 0;
}
