# include "pch.h"
# include "TCP_Client.h"

TCPClient::TCPClient()
{
	WSADATA wsaData;
	this->iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed: " << iResult << std::endl;
	}
	else
	{
		//this->createSocket();
		std::cout << "successfully creat a TCP object" << std::endl;
	}
}

// 设置IP和端口
void TCPClient::setIPandPort(string ip, int port)
{
	this->IP = ip;
	this->Port = port;
	std::cout << "设置成功" << ip << " " << port << std::endl;
	this->createSocket();
}

// 创建套接字
int TCPClient::createSocket()	
{
	this->clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	else
	{
		this->setServerAddress();
	}
}

// 设置服务器地址信息
void TCPClient::setServerAddress()  
{
	this->serverAddress;
	serverAddress.sin_family = AF_INET;  //IPv4
	serverAddress.sin_port = htons(Port); // 服务器端口
	inet_pton(AF_INET, IP.c_str(), &serverAddress.sin_addr); // 服务器 IP 地址
	this->connectServer();
}

// 连接到服务器
int TCPClient::connectServer()  
{
	this->iResult = connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
	if (this->iResult == SOCKET_ERROR) {
		std::cout << "connect failed: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		std::cout << "Connected to server." << std::endl;
	}
}

// 发送数据
void TCPClient::sendMsg(const char* sendbuf)  
{
	this->iResult = send(clientSocket, sendbuf, strlen(sendbuf), 0);
	if (this->iResult == SOCKET_ERROR) {
		std::cout << "send failed: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		WSACleanup();
	}
}

// 清除连接
void TCPClient::cleanTCP()
{
	closesocket(this->clientSocket);
}

// 接收数据
string TCPClient::recvMsg()
{
	char recvbuf[1024];
	this->iResult = recv(clientSocket, recvbuf, sizeof(recvbuf), 0);
	if (this->iResult > 0) {
		return string(recvbuf, iResult);
	}
	else if (this->iResult == 0) {
		std::cout << "Connection closed" << std::endl;
		return "1";
	}
	else {
		std::cout << "recv failed: " << WSAGetLastError() << std::endl;
		return "2";
	}
}