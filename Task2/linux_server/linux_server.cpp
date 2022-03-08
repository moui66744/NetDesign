#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include <netinet/in.h>       sockaddr_in, "man 7 ip" ,htons
#include <poll.h>              poll,pollfd
#include <arpa/inet.h>   //inet_addr,inet_aton
#include <unistd.h>        //read,write
#include <netdb.h>         //gethostbyname

#include <error.h>         //perror
#include <stdio.h>
#include <errno.h>         //errno

#include <string.h>          // memset
#include <string>

#include <pthread.h>
#include <verilator.h>

#define PORT_ID 6000


using namespace std;

string exec(const char *command){
	return "";	
}

int main(){
	//definition
	in_port_t ip = htons(PORT_ID);
	sockaddr_in serv_addr;
	sockaddr_in client_addr;
	
	//operation

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
	while(true){
		int tcp_conn = accept(serv_sock,(sockaddr*)&client_addr,sizeof(sockaddr));
		printf("Connection from :%s\n",inet_ntoa(client_addr.sin_addr));			
		pthread_create()
	}	
		
	close(serv_sock);
	return 0;
}
