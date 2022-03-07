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
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");          // 服务器地址
    addrSrv.sin_family = AF_INET;                                   // 使用TCP/IP
    addrSrv.sin_port = htons(6000);                                 // 服务器端口号
    connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));     // 向服务器发起建立连接的请求
    // 接收并显示建立连接的提示
    char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];
    fd_set fdread;
    memset(recvBuf, 0, BUF_SIZE);
    recv(sockClient, recvBuf, BUF_SIZE-1, 0);
    puts(recvBuf);
    /* 以上不需要变动 */
    // 接收用户输入，发送给服务器执行，并显示结果
    while (1) {
        // 读用户输入
        memset(sendBuf, 0, BUF_SIZE);
        gets_s(sendBuf);
        // 发送给服务器执行
        send(sockClient, sendBuf, strlen(sendBuf), 0);
        // 接收服务器执行结果
        // 由于输出可能很多，需要多次接收
        // 此处采用就绪通告 I/O 模型，详见 指导书P53，基本流程是固定的
        while (1) {
            // 等待时间
            timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            FD_ZERO(&fdread);
            FD_SET(sockClient, &fdread);
            int res = select(0, &fdread, NULL, NULL, &tv);// 超过等待时间，此时认为服务器没有更多数据，res = 0
            if (res <= 0) break;
            if (FD_ISSET(sockClient, &fdread)) {
                // 分批接收结果并输出
                memset(recvBuf, 0, BUF_SIZE);
                int len = recv(sockClient, recvBuf, BUF_SIZE-1, 0);
                printf("%s", recvBuf);
            }
        }
    }
    closesocket(sockClient);
	return 0;
}