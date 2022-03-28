#ifndef __LINUX_SERVER_H__
#define __LINUX_SERVER_H__

#include <sys/types.h>
#include <stdio.h>

const char * fifo_vtos = "./fifo_vtos"; 
//fifo from virtual model to server

const char * fifo_stov = "./fifo_stov";
//fifo from server to virtual model

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
#define FIFO_SEND_SIZE 18
//the length of data sent from server to virtual model

#define FIFO_RECV_SIZE 32
//the length of data server recieved from virtual model  

#endif //ifndef __LINUX_SERVER_H__
