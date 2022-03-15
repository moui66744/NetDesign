#include <iostream>
#include "test.h"
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>

using namespace std;
int msgid1,msgid2;
#define BUF_SIZE 2
char buf[BUF_SIZE];
int father_process(int pid){
	int data;
	printf("open fd_write\n");
	printf("This program will help you to calculate the value of x*x\n");
	while(true){
	scanf("%d",&data);
	int fd_write = open(fifo_stov,O_WRONLY,0);//pipe fd = file describer
	if (fd_write == -1){
		cerr << "Server : error occuried when open fifo write\n";
		exit(-1);
	}
	sprintf(buf,"%d",data);
	write(fd_write,buf,strlen(buf));
	printf("write data\n");
	int fd_read = open(fifo_vtos,O_RDONLY,0);
	
	if (fd_read == -1){
		cerr << "Server : error occuried when open fifo\n";
		exit(-1);
	}
	read(fd_read,buf,BUF_SIZE);
	sscanf(buf,"%d",&data);
	printf("The answer is %d\n",data);
	}
	int status;
	wait(&status);

	return 0;
}

int main(){
	if (access(fifo_vtos,F_OK) == -1){
		int res = mkfifo(fifo_vtos,0777);
		if (res == 0) {
			printf("fifo Created:%s\n",fifo_vtos);
		}
	}
	if (access(fifo_stov,F_OK) == -1){
		int res = mkfifo(fifo_stov,0777);
		if (res == 0) {
			printf("fifo_Created:%s\n",fifo_stov);
		}
	}
	int pid = fork();
	if (pid < 0){
		cerr << "error occuried when fork\n";
	}
	if (pid != 0){
		printf("Enter father_process\n");
		father_process(pid);	
	} else {
		execl("./build/vmodel",NULL);
	}

	return 0;
}
