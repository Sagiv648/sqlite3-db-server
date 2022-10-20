#ifndef _HANDLER_H_
#define _HANDLER_H_


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <Windows.h>
#include <sqlite3.h>
#include <WinSock2.h>
#include <queue>
#include "utils.hpp"
#include "packet.hpp"

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

}handler_info;


int setup_handlers(handler_info handlers[], SOCKET serverSocket);

int sqliteCallback(void* table, int count, char** colData, char** colName);

int readTable(table_info* tName);

#endif // !HANDLER_H_
