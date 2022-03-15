#include<verilated.h>
#include <iostream>
#include "Vtop.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "linux_server.h"
#include <assert.h>

using namespace std;

/*
 * FPVGA:Field Programmable Virtual Gate Array
 */
#define SW_NUM 16
#define LED_NUM 16
#define BUFF_SIZE 512
#define MSG_SIZE 32

#define MSG_KEY_A 9527
#define MSG_KEY_B 9528
TOP_NAME *top;
VerilatedContext *contextp;
int msg_id_1;
int msg_id_2;
int fd_read,fd_write;

static char buf_recv[BUFF_SIZE];
static char buf_send[BUFF_SIZE];
static unsigned short sw = 0;
static unsigned short led = 0;


bool fpvga_get_data(int sock){
	memset(buf_recv,0,BUFF_SIZE*sizeof(char));
	//recv(sock,buf_recv,BUFF_SIZE-1,0);
	printf("verilator:getting the data\n");
	fd_read = open(fifo_stov,O_RDONLY);
	read(fd_read,buf_recv,FIFO_MSG_SIZE);
	close(fd_read);
	printf("%s\nENDENDEND\n",buf_recv);
	if (strncmp(buf_recv,"EREQ",4) == 0){
		return false;		
	}
	sw = 0;
	char * queue = buf_recv;
	for(int i=0;i<SW_NUM;i++){
		sw += ((queue[i] - '0') << (SW_NUM-i-1));
		//(sw <<= 1) += queue][i] - '0';
		printf("i=%d,sw=%d,queue[i]=%c\n",i,sw,queue[i]);
	}
	printf("%d\n",sw);
	return true;
}
void fpvga_send_data(int sock){
	fd_write = open(fifo_vtos,O_WRONLY);
	printf("led=%x,%d\n",led,led);
	for (int i = 0;i < LED_NUM ; i++){
		buf_send[i] = ((led >> 15 - i)& 1) + '0';
		printf("i=%d:%d\n",i,buf_send[i]);
	}
	buf_send[LED_NUM] = '\0';
	write(fd_write,buf_send,16);
	close(fd_write);
#ifndef USE_MAIN
//	send(sock,buf_send,BUFF_SIZE-1,0);
#endif
	printf("%s\n",buf_send);
}
void fpvga_init() {
	contextp = new VerilatedContext;
	top = new TOP_NAME{contextp };
}

void fpvga_single(){
	printf("single test\n");
		top->sw = sw;
		top->eval();
		led = top->led;
		printf("In this test,sw=%d,led=%d\n",sw,led);
}
void fpvga_clear()
{
	if (top !=nullptr){
		delete top;
	}
	if (contextp != nullptr){
		delete contextp;
	}
}
int main(){
	int sock = 0;
	(void)freopen("test.out","w",stdout);
	printf("first line\n");

	fpvga_init();
	printf("done with init\n");
	while(fpvga_get_data(sock)){
		printf("get data success!\n");
		fpvga_single();
		fpvga_send_data(sock);
	}

	fpvga_clear();
	return 0;
}
