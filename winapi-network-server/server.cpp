
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "utils.hpp"
#include "network.hpp"


#include "handler.hpp"

#pragma comment(lib, "Ws2_32.lib")



#define TEST_ADDR "127.0.0.1"
#define TEST_PORT 7777
#define TEST_PORT_STR "7777"

sockaddr_in address;

SOCKET server_setup(unsigned short port, unsigned long addr );

void writeToFileTest(char* buffer);

handler_info handlers[MAX_HANDLERS];


std::queue<SOCKET> clientsQueue;

EntryArguments args;

int main(int argc, char** argv) {


	memset(&args, 0, sizeof(args));

	parseArguments(argv, argc, &args);


	SOCKET serverSocket = server_setup(args.port, args.ipv4);
	
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
	 
	 while (true) {
		 sockaddr connectedAddress;
		 ZeroMemory(&connectedAddress, sizeof(connectedAddress));
		 int connectedAddrSize = 0;
		 SOCKET clSocket = accept(serverSocket, &connectedAddress, &connectedAddrSize);
		 
		 if (clSocket == INVALID_SOCKET) {
			 cout << "Invalid client connection with winsocket error: " << WSAGetLastError() << '\n';
			 continue;
		 }
		 clientsQueue.push(clSocket);
		 handlers_scheduler(handlers, clientsQueue);
		
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

//void writeToFileTest(char* buffer) {
//
//	HANDLE file = CreateFileA("test.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (file == INVALID_HANDLE_VALUE) {
//		cout << "Error while opening file " << GetLastError() << '\n';
//		ExitProcess(1);
//	}
//	DWORD numWritten;
//	if (WriteFile(file, buffer, strlen(buffer), &numWritten, NULL) == 0) {
//		cout << "Error while writing " << GetLastError() << '\n';
//		ExitProcess(1);
//	}
//	CloseHandle(file);
//}

SOCKET server_setup(unsigned short port, unsigned long addr) {

	WSADATA sockInitData;

	if (WSAStartup(MAKEWORD(2, 2), &sockInitData) != 0) {
		std::cout << "Server init failed with winsocket error " << WSAGetLastError() << '\n';
		WSACleanup();
		return INVALID_SOCKET;
	}
	
	
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	if (args.isAutomatic) {

		
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		
		char buffer[7];
		_itoa_s(port, buffer, 10);
		buffer[strlen(buffer)] = 0;

		if (getaddrinfo(NULL, buffer, &hints, &result) != 0) {
			cout << "Error with automatic addressing with winsocket error " << WSAGetLastError();
			ExitProcess(1);
		}
		
	}
	else {
		memset(&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		address.sin_addr.S_un.S_addr = htonl(addr);

	}
	
	
	
	
		

	


	SOCKET serverSocket = INVALID_SOCKET;

	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		std::cout << "Server creation failed with winsocket error " << WSAGetLastError() << '\n';
		WSACleanup();
		return INVALID_SOCKET;
	}
	std::cout << "Server created successfully\n";



	if (bind(serverSocket, args.isAutomatic ? result->ai_addr : (sockaddr*)&address, 
		args.isAutomatic ?  (int)result->ai_addrlen :  sizeof(address)) == SOCKET_ERROR) {
		std::cout << "Server failed to bind with winsocket error " << WSAGetLastError() << '\n';
		closesocket(serverSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	


	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Server failed to listen with winsocket error " << WSAGetLastError() << '\n';
		closesocket(serverSocket);
		WSACleanup();

		return INVALID_SOCKET;
	}
	std::cout << "Server is up and running.\n";

	return serverSocket;
}

