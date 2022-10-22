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

typedef struct {

	packet p;
	table_info t;


} handler_input;

typedef struct {
	HANDLE hHandler;
	DWORD threadId;
	handler_input handlerInput;
	SOCKET socket;
	bool free;

}handler_info;


int setup_handlers(handler_info handlers[], SOCKET serverSocket);

int handlers_scheduler(handler_info handlers[], char* packetBuffer);

int sqliteCallbackRead(void* table, int count, char** colData, char** colName);

int readTable(table_info* table);

int writeTable(table_info* table);

#endif // !HANDLER_H_
