#include <iostream>
#include <string>
#include <cstring>
#include <direct.h>
#include "winsock2.h"
#include <windows.h>
#define BUF_SIZE 1024
#pragma comment(lib, "ws2_32.lib") 
#pragma warning(disable:4996)
using namespace std;

/**
* TODO: ? CMD -> shell
* TODO: windows -> linux
* TODO: �����У�����·�� + >
* TODO: ǰ����������Ƿ�������
* TODO: �Ͽ�����
*/

string exec(const char* command) {
	FILE* pipe = _popen(command, "r");
	string result = "";
	char buffer[512];
	while (!feof(pipe)) {
		if (fgets(buffer, 512, pipe) != NULL)// ĳ����˵ �˴�fgets�����л�����⣬�ɻ���fread
			result += buffer;
	}
	_pclose(pipe);
	return result;
}

string process(char *input, char *workDir, char *programDir) {
	string command = input;
	chdir(workDir);
	string result = "";
	if (command.find("cd ") == 0) {
		// Ŀǰֻ���þ���·��������һЩ��ֵ�����
		
		result = exec(command.append(" 2>&1").c_str());
		getcwd(workDir, sizeof(workDir));
		printf("in process, workDir: %s\n", workDir);
		
		//int pos = command.find(" ");
		//string path = command.substr(pos + 1, command.length() - pos - sizeof(" 2>&1"));
		//printf("cd path: %s\n", path.c_str());
		//if (chdir(path.c_str()) != 0) {
		//	// TODO: ·����Ч
		//	printf("·����Ч\n");
		//}
		//else {
		//	getcwd(workDir, sizeof(workDir));
		//	printf("�ı�·��\n");
		//}
	}
	else {
		result = exec(command.append(" 2>&1").c_str());
	}
	chdir(programDir);
	return result;
}

DWORD WINAPI socketHander(void* p) {
	SOCKET sock = *(int*)p;
	// ������ʾ��֪ͨ�ͻ����Ѿ����ӵ�������
	char sendBuf[BUF_SIZE], recvBuf[BUF_SIZE];
	char workDir[512], programDir[512];
	getcwd(workDir, sizeof(workDir) - 1);
	getcwd(programDir, sizeof(programDir) - 1);
	sprintf(sendBuf, "Welcome to Server");
	send(sock, sendBuf, strlen(sendBuf), 0);

	while (1) {
		memset(recvBuf, 0, BUF_SIZE);
		recv(sock, recvBuf, BUF_SIZE - 1, 0);
		printf("catch command: %s\n", recvBuf);
		string result = process(recvBuf, workDir, programDir);
		send(sock, result.c_str(), result.length(), 0);
		printf("current path: %s\n", workDir);
	}
	// �ر�����
	closesocket(sock);
	return 0;
}

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
		printf("Connection from: %s\n", inet_ntoa(addrClient.sin_addr));
		HANDLE handle = CreateThread(NULL, 0, socketHander, &sockConn, 0, NULL);
		WaitForSingleObject(handle, INFINITE);
	}
	WSACleanup();
	return 0;
}
