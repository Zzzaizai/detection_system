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
	int createSocket();  //创建套接字
	void setServerAddress();  //设置服务器地址信息
	int connectServer();  //连接到服务器
	void sendMsg(const char* sendbuf);  //发送数据
	void setIPandPort(string ip, int port);
	void cleanTCP();
	string recvMsg();

	int iResult;
	SOCKET clientSocket;
	sockaddr_in serverAddress;
	string IP;
	int Port;
};