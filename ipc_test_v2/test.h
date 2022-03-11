#ifndef __TEST_H__
#define __TEST_H__
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define MSG_SIZE 512

struct msg_item_t{
	long int type;//this data descibe the type of msg	
	char msg[MSG_SIZE];
};

const key_t msg_key_stov = 1234;
const key_t msg_key_vtos = 2345;

const long int msg_type = 1;

const char * fifo_vtos = "./fifo_vtos";
const char * fifo_stov = "./fifo_stov";
#endif //__TEST_H__
