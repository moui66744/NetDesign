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

#include <sys/ipc.h>
#include <sys/msg.h>

#include <fstream>

#define PORT_ID 6000
#define OP_SIZE 64
#define BUF_SIZE 512
#define MSG_SIZE 32
#define MSG_KEY_A 9527
#define MSG_KEY_B 9528

using namespace std;

struct msg_item_t{
	long int type = 4;
	char msg[MSG_SIZE];
}msg_item;
char buf_send[BUF_SIZE],buf_recv[BUF_SIZE],buf_cmd[BUF_SIZE],buf_res[BUF_SIZE];
void fpvga_interface(int sock){
	int msg_id_1 = msgget((key_t)MSG_KEY_A,0666|IPC_CREAT);
	int msg_id_2 = msgget((key_t)MSG_KEY_B,0666|IPC_CREAT);
	execl("make","vmodel",NULL);
	printf("end with buid\n");
	FILE* fp = popen("obj_dir/vmodel > test.out","w");
	while(true){
		scanf("%s",buf_recv);
		printf("recieve the string :%s\n",buf_recv);
		fprintf(fp,"%s\n",buf_recv);
		printf("send data:%s\n",buf_recv);
		msgrcv(msg_id_2,&(msg_item),sizeof(msg_item_t),0,0);
		strcpy(buf_send,msg_item.msg);
		printf("%s\n",buf_send);
	}
}

string process(char *cmd){
	memset(buf_send,0,BUF_SIZE*sizeof(char));
	FILE* fp;
	string res;
	printf("in process :cmd =[%s]\n",cmd);
	fp = popen(cmd,"r");
	while(feof(fp) == 0){
		memset(buf_res,0,BUF_SIZE*sizeof(char));
		fread(buf_res,1,BUF_SIZE-1,fp);	
		printf("in function process:buf_res=%s\n",buf_res);
		res.append(buf_res);
	}
	res.append("ERESP\n");
	printf("buffer_recv:%s\nbuffer_send%s\n",buf_recv,buf_send);
	pclose(fp);
	printf("live so far\n");
	return res;
}
void* socket_handler(int p ){
	int sock = p;
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
			fpvga_interface(sock);
		} else {
			printf("command:%s %s\n",op,buf_recv);
			memset(buf_cmd,0,BUF_SIZE);
			sprintf(buf_cmd,"%s 2>&1",buf_recv);
			string res = process(buf_cmd);
			printf("in function socket handler:\nres=%s\n",res.c_str());
			send(sock,res.c_str(),res.size(),0);
			//send(sock,buf_recv,strlen(buf_recv),0);
		}
	}
	close(sock);
	return NULL;
}



int main(){
	//definition
	//in_port_t ip = htons(PORT_ID);
	//sockaddr_in serv_addr;
	//sockaddr_in client_addr;
	//pthread_t pt;
	////operation
	//cout << "==================init =========================\n\n\n";
	//int serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//if (serv_sock < 0 ) {
	//	cout << "error when creating the socket";
	//	return 0;
	//}	
	//
	//serv_addr.sin_family = AF_INET;
	//serv_addr.sin_port = ip;
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//
	//int res =  bind(serv_sock,(sockaddr*)&serv_addr,sizeof(sockaddr_in));
	//if (res < 0){
	//	cout << "error when bind the socket";
	//}
	//	
	//res = listen(serv_sock,5);
	//socklen_t socklen = sizeof(sockaddr);
	////while(true){
	//	int tcp_conn = accept(serv_sock,(sockaddr*)&client_addr,&socklen);
	//	printf("Connection from :%s\n",inet_ntoa(client_addr.sin_addr));			
	//	pthread_create(&pt,NULL,socket_handler,&tcp_conn);
	////}
	////socket_handler(&tcp_conn);
	//	
	//void * useless;
	//pthread_join(pt,&useless);
	//close(serv_sock);
	printf("verilator:being the program\n");
	fpvga_interface(0);
	return 0;
}
