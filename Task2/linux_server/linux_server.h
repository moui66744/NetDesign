#ifndef __LINUX_SERVER_H__
#define __LINUX_SERVER_H__

#include <sys/types.h>
const key_t msg_key_stov = 1234;
const key_t msg_key_vtos = 2345;

const long int msg_type = 1;

const char * fifo_vtos = "./fifo_vtos";
const char * fifo_stov = "./fifo_stov";

#endif //ifndef __LINUX_SERVER_H__
