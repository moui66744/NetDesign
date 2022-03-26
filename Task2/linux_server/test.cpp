/**
 * @file test.cpp
 * @author zakilim
 * @brief 
 * 	this function used for local test without the internet connection. 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "linux_server.h"

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> //    sockaddr_in, "man 7 ip" ,htons
#include <poll.h>		//     poll,pollfd
#include <arpa/inet.h>	//inet_addr,inet_aton
#include <unistd.h>		//read,write
#include <netdb.h>		//gethostbyname

#include <error.h> //perror
#include <stdio.h>
#include <errno.h> //errno

#include <string.h> // memset
#include <string>

#include <pthread.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <fstream>
#include <assert.h>

using namespace std;

#define BUF_SIZE 512
char buf_write[BUF_SIZE], buf_read[BUF_SIZE];

int father_process(int pid)
{
	printf("This program will help you to calculate the value of x*x\n");
	int i = 0;
	while (i++ < 20)
	{
		int fd_write = open(fifo_stov, O_WRONLY, 0); // pipe fd = file describer
		if (fd_write == -1)
		{
			cerr << "Server : error occuried when open fifo write\n";
			exit(-1);
		}
		sprintf(buf_write, "111000100010001000");
		write(fd_write, buf_write, FIFO_SEND_SIZE);
		printf("write data\n");
		int fd_read = open(fifo_vtos, O_RDONLY, 0);

		if (fd_read == -1)
		{
			cerr << "Server : error occuried when open fifo\n";
			exit(-1);
		}
		read(fd_read, buf_read, FIFO_RECV_SIZE);
		printf("the input is %s\nThe answer is %s\n", buf_write, buf_read);
	}
	int fd_write = open(fifo_stov, O_WRONLY, 0); // pipe fd = file describer
	if (fd_write == -1)
	{
		cerr << "Server : error occuried when open fifo write\n";
		exit(-1);
	}
	sprintf(buf_write, "EREQ");
	write(fd_write, buf_write, FIFO_RECV_SIZE);
	printf("the input is %s\nThe answer is %s\n", buf_write, buf_read);
	int status;
	wait(&status);

	return 0;
}

int main()
{
	if (access(fifo_vtos, F_OK) == -1)
	{
		int res = mkfifo(fifo_vtos, 0777);
		if (res == 0)
		{
			printf("fifo Created:%s\n", fifo_vtos);
		}
	}
	if (access(fifo_stov, F_OK) == -1)
	{
		int res = mkfifo(fifo_stov, 0777);
		if (res == 0)
		{
			printf("fifo_Created:%s\n", fifo_stov);
		}
	}
	int pid = fork();
	// int pid = 0;
	if (pid < 0)
	{
		cerr << "error occuried when fork\n";
	}

	if (pid != 0)
	{
		printf("Enter father_process\n");
		father_process(pid);
	}
	else
	{
		printf("test program\n");
		execl("./obj_dir/vmodel", NULL);
	}

	return 0;
}