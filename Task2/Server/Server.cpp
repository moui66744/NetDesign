#include <iostream>
#include <string>
#include <cstring>
#include "winsock2.h"
#define BUF_SIZE 1024
#pragma comment(lib, "ws2_32.lib") 
#pragma warning(disable:4996)
using namespace std;

/**
* TODO: ? CMD -> shell
* TODO: windows -> linux
* TODO: 输入行：绝对路径 + >
* TODO: 前后关联操作是否正常？
*/

int main() {
	// 加载套接字
	WSADATA wsaDate;
	WSAStartup(MAKEWORD(2, 2), &wsaDate);
	// 创建监听套接字
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	// 绑定套接字
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	// 监听客户端请求
	listen(sockSrv, 5);

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	printf("%s\n", "Server is started...");
	while (1) {
		// 接收客户端请求
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		// 发送提示，通知客户端已经连接到服务器
		char sendBuf[BUF_SIZE], recvBuf[BUF_SIZE];
        sprintf(sendBuf, "Welcome to Server, user IP: %s", inet_ntoa(addrClient.sin_addr));
        send(sockConn, sendBuf, strlen(sendBuf), 0);
		while (1) {
			memset(recvBuf, 0, BUF_SIZE);
			recv(sockConn, recvBuf, BUF_SIZE-1, 0);
			printf("catch command: %s\n", recvBuf);
			FILE* pipe = _popen(recvBuf, "r");
			while (int len = fread(sendBuf, 1, BUF_SIZE-1, pipe)) {
				send(sockConn, sendBuf, len, 0);
			}
			_pclose(pipe);
		}
		// 关闭连接
		closesocket(sockConn);
	}
	return 0;
}
