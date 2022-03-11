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
const char* fifo_name = "./pipe";
struct msg_item_t {
	long int type;
	char msg[MSG_SIZE];
}msg_item;
static char buf_recv[BUFF_SIZE]="d1000100010001000";
static char buf_send[BUFF_SIZE];
static unsigned short sw = 0;
static unsigned short led = 0;
bool fpvga_get_data(int sock){
#ifndef USE_MAIN
	memset(buf_recv,0,BUFF_SIZE*sizeof(char));
	//recv(sock,buf_recv,BUFF_SIZE-1,0);
	printf("verilator:getting the data\n");
	msgrcv(msg_id_1,&msg_item,sizeof(msg_item_t),0,0);
	strcpy(buf_recv,msg_item.msg);
	printf("verilator got data%s\n",buf_recv);
#endif
	char * queue = buf_recv;
#ifdef USE_MAIN
	printf("led:%s,queue[0]=%c\n",buf_recv,queue[0]);
#endif	
	if (queue[0] == 'd'){
		queue = queue + 1;
		for(int i=0;i<SW_NUM;i++){
#ifdef USE_MAIN
			printf("i=%d,queue[i]=%d,num=%d\n",i,queue[i],((queue[i] - '0') << (SW_NUM-i+1)));
#endif
			sw += ((queue[i] - '0') << (SW_NUM-i+1));
		}
		printf("%d\n",sw);
		return true;
	}else if (queue[0] == 'e'){
		return false;
	}
	return false;
}
void fpvga_send_data(int sock){
	printf("led=%x,%d\n",led,led);
	for (int i = 0;i < LED_NUM ; i++){
		buf_send[i] = ((led >> i)& 1) + '0';
		printf("i=%d:%d\n",i,buf_send[i]);
	}
	buf_send[LED_NUM] = '\0';
	strcpy(msg_item.msg,buf_send);
	msgsnd(msg_id_2,&msg_item,sizeof(msg_item_t),0);
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
int main(int sock){
	msg_id_1 = msgget((key_t)MSG_KEY_A,0666|IPC_CREAT);
	msg_id_2 = msgget((key_t)MSG_KEY_B,0666|IPC_CREAT);
	FILE *fp = open(fifo_name,O_RDONLY);
	stdin = fp;
	freopen("test.out","w",stdout);
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
