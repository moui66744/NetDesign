#include <iostream>
#include <string>
#include "winsock2.h"
#define BUF_SIZE 1024
#pragma comment(lib, "ws2_32.lib") 
#pragma warning(disable:4996)
using namespace std;
int main() {
	// 加载套接字
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // 与服务器建立连接
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6000);
    connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    // 接收并显示建立连接的提示
    char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];
    fd_set fdread;
    memset(recvBuf, 0, BUF_SIZE);
    recv(sockClient, recvBuf, BUF_SIZE-1, 0);
    puts(recvBuf);
    while (1) {
        memset(sendBuf, 0, BUF_SIZE);
        gets_s(sendBuf);
        //strcat(sendBuf, " 2>&1");
        send(sockClient, sendBuf, strlen(sendBuf), 0);
        while (1) {
            timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            FD_ZERO(&fdread);
            FD_SET(sockClient, &fdread);
            int res = select(0, &fdread, NULL, NULL, &tv);
            if (res <= 0) break;
            if (FD_ISSET(sockClient, &fdread)) {
                memset(recvBuf, 0, BUF_SIZE);
                int len = recv(sockClient, recvBuf, BUF_SIZE-1, 0);
                printf("%s", recvBuf);
            }
        }
    }
    closesocket(sockClient);
	return 0;
}