#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>   //    sockaddr_in, "man 7 ip" ,htons
#include <poll.h>         //     poll,pollfd
#include <arpa/inet.h>   //inet_addr,inet_aton
#include <unistd.h>        //read,write
#include <netdb.h>         //gethostbyname

#include <error.h>         //perror
#include <stdio.h>
#include <errno.h>         //errno

#include <string.h>          // memset
#include <string>

#include <pthread.h>
#include <verilated.h>

#include <fstream>

#define PORT_ID 6000
#define OP_SIZE 64
#define BUF_SIZE 512

using namespace std;
char buf_send[BUF_SIZE],buf_recv[BUF_SIZE],buf_cmd[BUF_SIZE];
void fpvga(int sock);

int process(char *cmd){
	memset(buf_send,0,BUF_SIZE*sizeof(char));
	FILE* fp;
	fp = popen(buf_recv,"r");
	fgets(buf_recv,BUF_SIZE,fp);
	printf("%s\n",buf_recv);
	pclose(fp);
	return 0;
}
void* socket_handler(void *p){
	int sock = *(int*)p;
	printf("Welcome to Server\n");
	sprintf(buf_send,"Welcome to Server");
	//send(sock,buf_recv,strlen(buf_send),0);
	char op[OP_SIZE];	
	while(true){
		memset(buf_recv,0,BUF_SIZE*sizeof(char));
		printf("Ready to recv\n");
		recv(sock,buf_recv,BUF_SIZE-1,0);
		sscanf(buf_recv,"%s",op);
		printf("op:%s\n",op);
		if (strcmp(op,"exit") == 0){
			printf("exit\n");
			break;
		} else if (strcmp(op,"fpvga") == 0){
			printf("fpvga launching\n");
			sprintf(buf_send,"fpvga enable");
			send(sock,buf_recv,strlen(buf_send),0);
			fpvga(sock);
		} else {
			printf("command:%s\n",buf_recv);
			process(buf_recv);
			send(sock,buf_recv,strlen(buf_send),0);
		}
	}
	close(sock);
	return NULL;
}



int main(){
	//definition
	in_port_t ip = htons(PORT_ID);
	sockaddr_in serv_addr;
	sockaddr_in client_addr;
	pthread_t pt;
	//operation
	cout << "==================init =========================\n\n\n";
	int serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (serv_sock < 0 ) {
		cout << "error when creating the socket";
		return 0;
	}	

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = ip;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int res =  bind(serv_sock,(sockaddr*)&serv_addr,sizeof(sockaddr_in));
	if (res < 0){
		cout << "error when bind the socket";
	}
		
	res = listen(serv_sock,5);
	socklen_t socklen = sizeof(sockaddr);
	//while(true){
		int tcp_conn = accept(serv_sock,(sockaddr*)&client_addr,&socklen);
		printf("Connection from :%s\n",inet_ntoa(client_addr.sin_addr));			
		pthread_create(&pt,NULL,socket_handler,&tcp_conn);
	//}
	//socket_handler(&tcp_conn);
	
	void * useless;
	pthread_join(pt,&useless);
	close(serv_sock);
	return 0;
}
