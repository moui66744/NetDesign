#include<iostream>
#include<string.h>
#include<sys/socket.h>

#include<verilator.h>
using namespace std;

#define OP_SIZE 64
#define BUF_SZIE 512

/*
* FPVGA:Field Programmable Virtual Gate Array
*/
TOP_NAME *top;
#define GET_BIT(x,num) (num>>x)&1 
void fgvpa_get_data();
void fgvpa_init() {
	
}
int get_sw(int i){
	
}

void fgvpa_single(void p){
	
	for (int i = 0;i < SW_NUM; i ++){
		top->sw[i] = get_sw(i);
	}
	for (int i = 0;i < LED_NUM; i++){
		out[i] = top->led[i];
	}
}

void socket_handler(void *p){
	int sock = *(int*)p;
	char buf_send[BUF_SIZE],buf_recv[BUF_SIZE];
	sprintf(buf_send,"Welcome to Server");
	//function notes:
	//	send(int socket,char*,int len,int type)
	send(sock,buf_send,strlen(buf_send),0);
	while(true){
		memset(buf_recv,0,BUF_SIZE);
		recv(sock,buf_recv,BUF_SIZE-1,0);
		char op[OP_SIZE];
		sscanf(buf_recv,"%s",op);
		if (strcmp(op,"exit") == 0){
			printf("exit");
			break;
		} else if (strcmp(op,"fgvpa")){
			fgvpa(sock);
		} else {
			
		}
	}
	close(sock);
}


int main(){

	return 0;
}
