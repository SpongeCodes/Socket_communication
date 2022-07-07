// C++ program for creating a socket using Winsock API
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<tchar.h>
#include<chrono>
#include<filesystem>
// winsock api header file
#include <winsock2.h>
// tcp/ip functionality
#include<ws2tcpip.h>

#define MAX_SIZE 1024

#pragma comment(lib, "Ws2_32.lib")

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::ifstream;
using std::ofstream;
namespace fs = std::filesystem;

// maybe create a class
// that has field_name, field_type, and value fields.
// create object of this class and pass this entire object through send function

// disregard above comments
// send data through a buffer and read it out accordingly
// so for each line of text send field_name and print data value;


// template

// based on data type in second field required to pass that particular data type

/*
*  template<typename T>
	struct Data {
		string field_name;
		T field_value;

	}
*/


//#include "SQLAPI.h"		 // main SQLAPI

int	main(int c, char* argv[])
{
	printf("Hello!\n");

	// Initialize winsock by loading winsock DLL
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2,2);
	int wsa_err;
	wsa_err = WSAStartup(wVersionRequested,&wsaData);

	// create server socket
	SOCKET server_socket =  INVALID_SOCKET;
    server_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	/*if (wsa_err != 0)
	{
		cout << "The winsock dll not found!" << endl;
	}

	else {
		cout << "The winsock dll is found!" << endl;
		cout << "The system status: "<<wsaData.szSystemStatus << endl;
	}*/

	if (server_socket != INVALID_SOCKET)
		cout << "socket "<<server_socket <<" is working" << endl;

	// bind socket to an IP address and port number
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET,_T("127.0.0.1"), &service.sin_addr.s_addr);
	int port = 55555;
	service.sin_port = htons(port);

	char s[INET6_ADDRSTRLEN];

	if (bind(server_socket, (sockaddr*)&service, sizeof(service)) != SOCKET_ERROR)
	{
		inet_ntop(AF_INET, &service.sin_addr.s_addr,s,sizeof(s));
		cout << "bind is working at IP address " << s << " and port " << service.sin_port << endl; //printf("Bind is working at IP address");
	}
	// listen for incoming connections

	if (listen(server_socket, 1) != SOCKET_ERROR) cout << "Listening for connections" << endl;
	else cout << WSAGetLastError() << endl;

	// accept connection(s) from incoming client(s)
	SOCKET accept_socket = accept(server_socket,NULL,NULL);
	if (accept_socket != SOCKET_ERROR) cout << "Connected to client" << endl;

	//system("pause");
	//closesocket(server_socket);

	// READ AND WRITE

	// send an integer and a float 
	//int a = 15; float f = 10.57;

	unsigned char data_buffer[MAX_SIZE]{};
	int ptr = 0;
	

	//	memcpy(data_buffer+ptr,&a,sizeof(int));
	//	ptr += sizeof(int);
	//	memcpy(data_buffer + ptr, &f, sizeof(float));

	//	//cout << "float value is " <<(float)(data_buffer+ptr) << "\n";
	//send(accept_socket, (char*)data_buffer, sizeof(data_buffer), NULL);

	//send(accept_socket, (char*)&f, sizeof(float), NULL);
	while (1)
	{
		ptr = 0; // start from beginning of buffer for every message
		/*for (auto& p : fs::directory_iterator("messages"))
			cout << p << endl;*/
		cout << "Enter the file name you want to send or enter exit to exit the program!" << endl;
		string file_choice;
		cin >> file_choice;
		if (file_choice == "exit")
		{
			cout << "Exiting out of program"<<endl;
			break;
		}
		bool flag = 0;
		fs::path chosen_path = "messages/" + file_choice + ".txt";
		cout <<"Chosen path is: " << chosen_path << endl;
		for (auto& p : fs::directory_iterator("messages"))
		{
			if (chosen_path == p) { flag = true; break; }

			
		}
			if(!flag) {
				cout << "File not found! Wait for administrator to add file. Else check for mistake in input" << endl;
				continue;
			}


			ifstream fin;
		fin.open(chosen_path);

		// First send a schema of particular document
		char file_schema[100]{};
		fin >> file_schema;
		//cout << "send: " << file_schema << endl;
		//printf("%s \n", file_schema);
		//int size = 
		/*if (strlen(file_schema) == 0) {
			cout << "File is empty!" << endl << endl;
			continue;
		}*/
		send(accept_socket, file_schema, strlen(file_schema), NULL);
		// also iterate over all text files in directory
		// 
		// 
		//char str[25] = "Hello from Upamanyu";
		//send(accept_socket,str,sizeof(str),NULL);

		// maybe add a first line to the text file to give the structure
		// send this to the client side so that it can read data in order
		string file_text;
		stringstream param(file_schema);
		while (fin >> file_text) {

			if (ptr >= MAX_SIZE) break; // stop reading to prevent overriding buffer

			//string file_text;
			cout << file_text << endl;
			stringstream ss(file_text);
			// TO DO 
			// extract field_name, type, and value
			string field_name, field_type, value;

			getline(ss, field_name, ',');
			getline(ss, field_type, ',');
			getline(ss, value, ',');

			getline(param, field_type, ',');

			// maybe change memcpy to memcpy_s
			// remove all if statements 
			if (field_type.substr(0, 4) == "CHAR")
			{
				//memcpy_s;
				int string_size = stoi(field_type.substr(4));
				value.resize(string_size);
				//cout << value << endl;
				memcpy(data_buffer + ptr, value.c_str(), string_size);
				ptr += string_size;
			}


			if (field_type.compare("INT") == 0)
			{
				int a = stoi(value);
				memcpy(data_buffer + ptr, &a, sizeof(int));
				//int b;
				//memcpy(&b, data_buffer + ptr, sizeof(int));
				//cout << "Integer copied is " << b << endl;
				ptr += sizeof(int);
			}

			if (field_type.compare("FLOAT") == 0)
			{
				float f = stof(value);
				memcpy(data_buffer + ptr, &f, sizeof(float));
				ptr += sizeof(float);
			}

			if (field_type.compare("BOOL") == 0)
			{
				bool b = value.compare("TRUE") == 0 ? true:false ;
				memcpy(data_buffer + ptr, &b, sizeof(bool));
				ptr += sizeof(float);
			}

			cout << "Field name is " << field_name << " field_type is " << field_type << " value is " << value << endl;

			//if (strcmp(file_text, "\n") == 0) { cout << "new line found" << endl; continue; }
			// write code to remove all newlines revise file programming
			//send(accept_socket, field_name.c_str(), sizeof(field_name), NULL);
		}
		// after reading into buffer send the complete buffer
		if (strlen(file_schema) != 0)
			send(accept_socket, (char*)data_buffer, sizeof(data_buffer), NULL);

		memset(data_buffer,0,MAX_SIZE);
		memset(file_schema,0,100);
		// free resources by de-registering winsock DLL
		//WSACleanup();
	}
	
	return 0;
}
