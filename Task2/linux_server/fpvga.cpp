/**
 * @file fpvga.cpp
 * @author zakilim
 * @brief 
 * 	this file contains the main function of virtual fpga
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
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


// the standard signal used in verilating the model 

static unsigned short sw = 0;
//sw : switchs
static unsigned short led = 0;
//led : leds
static unsigned short clk = 0;
//clk : clock signal
static unsigned short rst = 0;
//rst : reset button
static unsigned short seg = 0;

/**
 * @brief 
 * 
 * void fpvga_get_data()
 * 
 *	read data from the fifo, convert the string to the signals for verilating  
 * 
 */
bool fpvga_get_data(){
	memset(buf_recv,0,BUFF_SIZE*sizeof(char));
	//recv(sock,buf_recv,BUFF_SIZE-1,0);
	printf("verilator:getting the data\n");
	fd_read = open(fifo_stov,O_RDONLY);
	read(fd_read,buf_recv,FIFO_SEND_SIZE);
	//read the input data
	close(fd_read);
	printf("%s\nENDENDEND\n",buf_recv);
	if (strncmp(buf_recv,"EREQ",4) == 0){
		// EREQ : standard exit command from client
		// when this signal shows up, the fpvga will exit
		return false;		
	}
	sw = 0;
	clk = buf_recv[0] - '0';
	rst = buf_recv[1] - '0';
	// get the clk and the rst
	char * queue = buf_recv + 2;
	
	for(int i=0;i<SW_NUM;i++){
		sw += ((queue[i] - '0') << (SW_NUM-i-1));
		//(sw <<= 1) += queue][i] - '0';
		printf("i=%d,sw=%d,queue[i]=%c\n",i,sw,queue[i]);
	}
	printf("%d\n",sw);
	return true;
	//no need for exiting the program,return ture
}
/**
 * @brief 
 *
 * void fpvga_send_data();
 * 
 * convert the result to a string, and use fifo to send it back
 *  
 */
void fpvga_send_data(){
	fd_write = open(fifo_vtos,O_WRONLY);
	//open file describer
	printf("led=%x,%d\n",led,led);
	for (int i = 0;i < LED_NUM ; i++){
		buf_send[i] = ((led >> 15 - i)& 1) + '0';
		printf("i=%d:%d\n",i,buf_send[i]);
	}
	printf("led=%x,%d\n",led,led);
	for (int i = 0;i < LED_NUM ; i++){
		buf_send[i+16] = ((seg >> 15 - i)& 1) + '0';
		printf("i=%d:%d\n",i,buf_send[i+16]);
	}
	buf_send[LED_NUM*2] = '\0';
	write(fd_write,buf_send,FIFO_RECV_SIZE);
	//write data to fifo
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

/**
 * @brief 
 * 
 * void fpvga_single();
 * 
 * simulate once
 *
 */
void fpvga_single(){
	printf("single test\n");
		top->sw = sw;
		top->clk = clk;
		top->rst = rst;
		top->eval();
		led = top->led;
		seg = top->seg;
		printf("In this test,sw=%d,led=%d,seg=%d\n",sw,led);
}
/**
 * @brief 
 * 
 * void fpvga clear();
 * 
 * free the memory used by verilator 
 *  
 */

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
	(void)freopen("test.out","w",stdout);
	printf("first line\n");

	fpvga_init();
	printf("done with init\n");
	while(fpvga_get_data()){
		printf("get data success!\n");
		fpvga_single();
		fpvga_send_data();
	}

	fpvga_clear();
	return 0;
}
