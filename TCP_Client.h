#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

class TCPClient {
public:
	TCPClient();
	int createSocket();  //�����׽���
	void setServerAddress();  //���÷�������ַ��Ϣ
	int connectServer();  //���ӵ�������
	void sendMsg(const char* sendbuf);  //��������
	void setIPandPort(string ip, int port);
	void cleanTCP();
	string recvMsg();

	int iResult;
	SOCKET clientSocket;
	sockaddr_in serverAddress;
	string IP;
	int Port;
};