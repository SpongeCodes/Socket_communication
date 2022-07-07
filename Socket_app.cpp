// Socket_app.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// C++ program for connecting to database (and error handling)
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<chrono>
#include "SQLAPI.h"		 // main SQLAPI

//using namespace std::chrono;
using std::string;

int main(int c, char* argv[])
{
	printf("Hello!\n");
	// create connection object to connect to database
	SAConnection con;
	try
	{
		// connect to database
		// in this example, it is Oracle,
		// but can also be Sybase, Informix, DB2 
		// SQLServer, InterBase, SQLBase and ODBC
		con.Connect(_TSA("my_db"), // database name
			_TSA("root"), // user name
			_TSA("march2000"), // password
			SA_MySQL_Client); //MySQL Client
		printf("We are connected!\n");

		string text = "Hello from Upamanyu", name = "Upamanyu ";
		string address = "Salt Lake";
		int age = 22;

		SACommand insert(&con, _TSA("INSERT INTO MESSAGES (message_id,name,age,address,text,time) VALUES (uuid(),:name, :age, :address, :text, now())"));
		
		//auto local_time =  std::chrono::zoned_time{ std::chrono::current_zone(),  std::chrono::system_clock::now() };
		
		//SADateTime dtValue = local_time;
		
		insert.Param("name").setAsString() = name.c_str();
		insert.Param("age").setAsInt64() = age;
		insert.Param("address").setAsString() = address.c_str();
		insert.Param("text").setAsString() = text.c_str();
		insert.Execute();
		//insert << _TSA("Nick Barry") << 35L;
		//insert.Execute();*/

		// Disconnect is optional
		// autodisconnect will occur in destructor if needed
		/*con.Disconnect();
		printf("We are disconnected!\n");*/
	}

	catch (SAException& x)
	{
		printf("We are not connected!\n");
		// SAConnection::Rollback()
		// can also throw an exception
		// (if a network error for example),
		// we will be ready
		con.Rollback();
		// print error message
		printf("%s\n", (const char*)x.ErrText());
	}
	return 0;
}
