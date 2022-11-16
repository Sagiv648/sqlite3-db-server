#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <WinSock2.h>
#include <iostream>
#include <sqlite3.h>

#pragma comment(lib, "Ws2_32.lib")


#define PORT 7777
#define HOST "127.0.0.1"

#define BUFLEN 4096
sockaddr_in address;
const char buffer[BUFLEN] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
int main() {

	WSADATA sockInitData;
	if (WSAStartup(MAKEWORD(2, 2), &sockInitData) != 0) {
		std::cout << "Sock init failed with error " << WSAGetLastError() << '\n';
		WSACleanup();
		return 1;
	}



	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.S_un.S_addr = inet_addr(HOST);
	SOCKET clSocket = INVALID_SOCKET;

	if ((clSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		std::cout << "Socket creation failed with error " << WSAGetLastError() << '\n';
		closesocket(clSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Socket created successfully\n";

	if (connect(clSocket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
		std::cout << "Socket failed to connect with error " << WSAGetLastError() << '\n';
		closesocket(clSocket);
		WSACleanup();
		return 1;
	}
	char headerPacket[] = "{\r\nOp_Code:100\r\nPacket_Serial_Num:12345\r\nNext_Packet_Len:673423\r\nTransmition_Type:1\r\nDatabase:test.db\r\nTable_Name:students\r\n}";
	char tu[] = "Students";
	int sent;
	sent = send(clSocket, headerPacket, sizeof(headerPacket), 0);
	if (sent == SOCKET_ERROR) {
		std::cout << "Error:" << WSAGetLastError() << '\n';
	}
	std::cout << "Socket sent " << sent << " bytes\n";
	
	if (shutdown(clSocket, SD_SEND) == SOCKET_ERROR)
	{
		std::cout << "Socket failed to shutdown with error " << WSAGetLastError() << '\n';
		closesocket(clSocket);
		WSACleanup();
		return 1;
		
	}
	std::cout << "Socket successfully shut down\n";
	closesocket(clSocket);
	WSACleanup();

	return 0;
}