#ifndef _HANDLER_H_
#define _HANDLER_H_


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <sqlite3.h>
#include <WinSock2.h>
#include "utils.hpp"
#include "packet.hpp"
#include "network.hpp"

#define MAX_HANDLERS 5

DWORD mainHandler(void* handlerInput);

 struct handler_input {

	packet p;
	table_info t;
	SOCKET connected_socket;


};

 struct handler_info {
	HANDLE hHandler;
	DWORD threadId;
	handler_input handlerInput;
	SOCKET serverSocket;
	bool free;

	handler_info(HANDLE handle, DWORD tId, handler_input input, SOCKET servSocket, bool isFree) {
		hHandler = handle;
		threadId = tId;
		handlerInput = input;
		serverSocket = servSocket;
		free = isFree;
	}
	handler_info() {
		hHandler = 0;
		threadId = 0;
		serverSocket = 0;
		free = false;

	}
	

};


int setup_handlers(handler_info handlers[], SOCKET serverSocket);

int handlers_scheduler(handler_info handlers[], std::queue<SOCKET>& clientsQueue);

int sqliteCallbackRead(void* table, int count, char** colData, char** colName);

int sqliteCallbackReadTypes(void* table, int count, char** data, char** columns);

int readTable(table_info* table);

int writeTable(table_info* table);

#endif // !HANDLER_H_
