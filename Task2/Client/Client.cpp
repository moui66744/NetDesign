#include <iostream>
#include <string>
#include "winsock2.h"
#define BUF_SIZE 1024
#pragma comment(lib, "ws2_32.lib") 
#pragma warning(disable:4996)
using namespace std;
int main() {
	// �����׽���
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // ���������������
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");          // ��������ַ
    addrSrv.sin_family = AF_INET;                                   // ʹ��TCP/IP
    addrSrv.sin_port = htons(6000);                                 // �������˿ں�
    connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));     // ����������������ӵ�����
    // ���ղ���ʾ�������ӵ���ʾ
    char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];
    fd_set fdread;
    memset(recvBuf, 0, BUF_SIZE);
    recv(sockClient, recvBuf, BUF_SIZE-1, 0);
    puts(recvBuf);
    /* ���ϲ���Ҫ�䶯 */
    // �����û����룬���͸�������ִ�У�����ʾ���
    while (1) {
        // ���û�����
        memset(sendBuf, 0, BUF_SIZE);
        gets_s(sendBuf);
        // ���͸�������ִ��
        send(sockClient, sendBuf, strlen(sendBuf), 0);
        // ���շ�����ִ�н��
        // ����������ܺܶ࣬��Ҫ��ν���
        // �˴����þ���ͨ�� I/O ģ�ͣ���� ָ����P53�����������ǹ̶���
        while (1) {
            // �ȴ�ʱ��
            timeval tv;
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            FD_ZERO(&fdread);
            FD_SET(sockClient, &fdread);
            int res = select(0, &fdread, NULL, NULL, &tv);// �����ȴ�ʱ�䣬��ʱ��Ϊ������û�и������ݣ�res = 0
            if (res <= 0) break;
            if (FD_ISSET(sockClient, &fdread)) {
                // �������ս�������
                memset(recvBuf, 0, BUF_SIZE);
                int len = recv(sockClient, recvBuf, BUF_SIZE-1, 0);
                printf("%s", recvBuf);
            }
        }
    }
    closesocket(sockClient);
	return 0;
}