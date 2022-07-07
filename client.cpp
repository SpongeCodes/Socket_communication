// C++ program for creating a socket using Winsock API
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<tchar.h>
// winsock api header file
#include <winsock2.h>
// tcp/ip functionality
#include<ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using std::cout;
using std::endl;
using std::string;


//#include "SQLAPI.h"		 // main SQLAPI

int	main(int c, char* argv[])
{
	printf("Hello!\n");

	// Initialize winsock by loading winsock DLL
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int wsa_err;
	wsa_err = WSAStartup(wVersionRequested, &wsaData);

	// create client socket
	SOCKET client_socket = INVALID_SOCKET;
	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/*if (wsa_err != 0)
	{
		cout << "The winsock dll not found!" << endl;
	}

	else {
		cout << "The winsock dll is found!" << endl;
		cout << "The system status: "<<wsaData.szSystemStatus << endl;
	}*/

	if (client_socket != INVALID_SOCKET)
		cout << "socket " << client_socket << " is working" << endl;

	// connect client socket to server socket defined in another application

	sockaddr_in clientservice;
	clientservice.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientservice.sin_addr.s_addr);
	int port = 55555;
	clientservice.sin_port = htons(port);

	if (connect(client_socket, (sockaddr*)&clientservice, sizeof(clientservice)) != SOCKET_ERROR) {
		cout << "Connected to server" << endl;


	}

	//closesocket(client_socket);

	// free resources by de-registering winsock DLL
	//WSACleanup();
	
	return 0;
}
