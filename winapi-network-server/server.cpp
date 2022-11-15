
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <WinSock2.h>
#include <iostream>
#include <sqlite3.h>
#include <WS2tcpip.h>
#include "utils.hpp"
#include "network.hpp"
#include "packet.hpp"
#include "table.hpp"
#include "handler.hpp"

#pragma comment(lib, "Ws2_32.lib")



#define TEST_ADDR "127.0.0.1"



sockaddr_in address;

SOCKET server_setup(std::map<string, string> varMapping);

handler_info handlers[MAX_HANDLERS];

int main(int argc, char** argv) {

	string test = readFromFile();

	if (test == "") {
		ExitProcess(1);
	}
	
	std::map<string,string> varMapping = parseFromFile(test);
	
	if (varMapping.size() == 0) {
		ExitProcess(1);
	}



	SOCKET serverSocket = server_setup(varMapping);
	
	if (serverSocket == INVALID_SOCKET) {
		ExitProcess(1);
	}
	
	SOCKET cl = INVALID_SOCKET;
	sockaddr_in clAddress;
	memset(&clAddress, 0, sizeof(clAddress));
	char* buffer = new char[BUFLEN];
	ZeroMemory(buffer, BUFLEN);

	if (setup_handlers(handlers, serverSocket) != 0) {
		cout << "No memory for thread creation\n";
		ExitProcess(1);
	}
	
	
	//------------------------------------------------------ test section -------------------------------------------------------------------

	//Actual data packet:
/*
	{\r\n
		%(Column_Name_Placeholder_#1):(%(Column_Data_#1)|%(Column_Data_#2)|%(Column_Data_#N))\r\n
		%(Column_Name_Placeholder_#2):(%(Column_Data_#1)|%(Column_Data_#2)|%(Column_Data_#N))\r\n
		%(Column_Name_Placeholder_#N):(%(Column_Data_#1)|%(Column_Data_#2)|%(Column_Data_#N))\r\n
	}
	
*/
	 char dataPacket[] = "{\r\nName:timmy|johnny|kim\r\nAge:16|17|18\r\nClass:Computer science|Engineering|Art\r\n}";
	 char headerPacket[] = "{\r\nOp_Code:100\r\nPacket_Serial_Num:12345\r\nNext_Packet_Len:673423\r\nTransmition_Type:1\r\nDatabase:test.db\r\nTable_Name:students\r\n}";
	 char tu[] = "Students";
	 

	 while (true) {
		 sockaddr connectedAddress;
		 ZeroMemory(&connectedAddress, sizeof(connectedAddress));
		 int connectedAddrSize = sizeof(sockaddr);
		 char headerBuffer[BUFLEN / 16]; // 4 KiB test
		 ZeroMemory(headerBuffer, BUFLEN / 16);
		 int recved = 0;
		 int total = 0;
		 SOCKET clSocket = accept(serverSocket, NULL, NULL);
		 if (clSocket == INVALID_SOCKET) {
			 cout << "Invalid socket with error: " << WSAGetLastError() << '\n';
			 continue;
		 }
		 
		 while (handlers_scheduler(handlers, headerBuffer, clSocket) == 0);
		 

		 


		 
	 }


	 
	
	


	
	//------------------------------------------------------ test section -------------------------------------------------------------------

	//	//	Operations order for the server:
	//	/*	1) Accept a connection
	//	*	2) Once a connection is accepted, call a recv method to recieve the header
	//	*	3) Once the header was recieved in a buffer, call the method of the packet class 'recieveHeaderPacket' to parse it for: 1) packet_serial_num, 2) next_packet_len, 3) transmition_type, 4) table_name
	//	*	4) Save the details in a data structure to pass it to the sqlite3 handler thread
	//	*	5) The sqlite3 handler thread will generally use only: 1) transmition_type(requesting a table/writing to a table), 2) table_name(requested table/"dirty" table)
	//	*	*6) At operation 4) the server will return to the listening state and from there the sqlite3 handler thread will determine the number of bytes of the packet for the header
	//	*	7) Once the sqlite3 handler thread determined the number of bytes, the header could be sent with the client's request packet serial number and after that the data packet
	//	*	8) The sqlite3 handler thread will return to sleep until the main thread will wake it up
	//	*/
	//}
	  
	
	 

	

	closesocket(serverSocket);
	WSACleanup();
	return 0;
}

SOCKET server_setup(std::map<string,string> varMapping) {

	int PORT = std::stoi(varMapping["PORT"]);

	char HOST[16]; //purely to conform to how the api accepts only char* string types

	ZeroMemory(HOST, sizeof(HOST));
	size_t t = 0;
	for (t = 0; t < varMapping["HOST"].size(); ++t) {
		HOST[t] = varMapping["HOST"][t];
	}
	HOST[t] = 0;

	WSADATA sockInitData;

	if (WSAStartup(MAKEWORD(2, 2), &sockInitData) != 0) {
		std::cout << "Sock init failed with error " << WSAGetLastError() << '\n';
		WSACleanup();
		return INVALID_SOCKET;
	}

	struct addrinfo* result = NULL, * ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	//cout << varMapping["PORT"] << '\n';
	if (getaddrinfo(NULL, varMapping["PORT"].c_str(), &hints, &result) != 0) {
		cout << "error with getaddrinfo with error " << WSAGetLastError();
		ExitProcess(1);
	}



	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.S_un.S_addr = inet_addr(TEST_ADDR);
	SOCKET serverSocket = INVALID_SOCKET;

	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		std::cout << "Socket creation failed with error " << WSAGetLastError() << '\n';
		WSACleanup();
		return INVALID_SOCKET;
	}

	std::cout << "Socket created successfully\n";


	if (bind(serverSocket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		std::cout << "Socket failed to bind with error " << WSAGetLastError() << '\n';
		closesocket(serverSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	std::cout << "Socket bound successfully\n";


	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Socket failed to listen with error " << WSAGetLastError() << '\n';
		closesocket(serverSocket);
		WSACleanup();

		return INVALID_SOCKET;
	}
	std::cout << "Socket listening successfully\n";

	return serverSocket;
}

