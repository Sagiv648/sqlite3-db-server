#ifndef _HANDLER_H_
#define _HANDLER_H_


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "utils.hpp"
#include "network.hpp"
#include "Table.hpp"


#define MAX_HANDLERS 5

DWORD mainHandler(void* handlerInput);

 struct handler_input {

	
	Table t;
	SOCKET connected_socket;
	handler_input() {
		connected_socket = SOCKET_ERROR;
		
	}
	handler_input& operator=( handler_input& right) {
		return right;
	}


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

int readTable(Table* table);

int writeTable(Table* table);

#endif // !HANDLER_H_
