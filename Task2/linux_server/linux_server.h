#ifndef __LINUX_SERVER_H__
#define __LINUX_SERVER_H__

#include <sys/types.h>
#include <stdio.h>
const key_t msg_key_stov = 1234;
const key_t msg_key_vtos = 2345;

const long int msg_type = 1;

const char * fifo_vtos = "./fifo_vtos";
const char * fifo_stov = "./fifo_stov";

// void read_fifo(int fd,char * str){
//     char ch;
//     int i = 0;
//     do{
//         read(fd,&ch,1);
//         str[i++] = ch;
//     }while(ch !='\n');
//     str[i] = '\n';
// }
// void write_fifo(int fd,char *str,int length = -1){
//     int i = 0;
//     while(str[i] != '\n'||str[i] == '\0'){
//         write(fd,str,1);
//         printf("%d\n",i);
//         i++;
//     }
// }
#define FIFO_MSG_SIZE 16

#endif //ifndef __LINUX_SERVER_H__
