#include<verilated.h>
#include "Vtop.h"
#include <sys/socket.h>
using namespace std;

/*
 * FPVGA:Field Programmable Virtual Gate Array
 */
#define SW_NUM 16
#define LED_NUM 16
#define BUFF_SIZE 512
TOP_NAME *top;
VerilatedContext *contextp;

static char buf_recv[BUFF_SIZE];
static char buf_send[BUFF_SIZE];
static char sw = 0;
static char led = 0;
bool fpvga_get_data(int sock){
	memset(buf_recv,0,BUFF_SIZE);
	recv(sock,buf_recv,BUFF_SIZE-1,0);
	char * queue = buf_recv;
	printf("led:%s,queue[0]=%c\n",buf_recv,queue[0]);
	if (queue[0] == 'd'){
		queue = queue + 1;
		for(int i=0;i<SW_NUM;i++){
			printf("i=%d\n",i);
			sw |= (queue[i] - '0') << i;
		}
		printf("%d\n",sw);
		return true;
	}else if (queue[0] == 'e'){
		return false;
	}
	return false;
}
void fpvga_send_data(int sock){
	for (int i = 0;i < LED_NUM ; i++){
		buf_send[i] = (led >> i)& 1 + '0';
	}
	send(sock,buf_send,BUFF_SIZE-1,0);
}
void fpvga_init() {
	contextp = new VerilatedContext;
	top = new TOP_NAME{contextp };
}

void fpvga_single(){

		top->sw = sw;
		top->eval();
		led = top->led;
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
void fpvga(int sock){
	fpvga_init();
	printf("done with init\n");
	while(fpvga_get_data(sock)){
		printf("get data success!\n");
		fpvga_single();
		fpvga_send_data(sock);
	}
	fpvga_clear();
}
