#include "handler.hpp"



HANDLE writerMutex;

DWORD mainHandler(void* handlerInput) {

	
	handler_info* handler = (handler_info*)handlerInput;
	cout << "this is thread with id " << handler->threadId << "\n\n\n";
	char headerBuffer[1024];
	
	packet serverSender;
	
	//At this point, the client socket is mapped to the thread
	//recieve the header packet here

		int bytesSent = 0;
		int totalBytesSent = 0;
		bool firstEntry = true;
	// packet class and table_info class are initialized by the recieveHeaderPacket function
	while (true)
	{
		if (!firstEntry) {

			handler->handlerInput.p.~packet();
			handler->handlerInput.t.~table_info();
			handler->free = true;
			SuspendThread(handler->hHandler);
			cout << "thread was suspended but continued here\n";

		}
		

		ZeroMemory(headerBuffer, sizeof(headerBuffer));


		if (recv(handler->handlerInput.connected_socket, headerBuffer, sizeof(headerBuffer)-1, 0) == SOCKET_ERROR) {
			cout << "Failure to recieve data from the client with winsocket error " << WSAGetLastError() << '\n';
			
			if (firstEntry)
				firstEntry = !firstEntry;
			continue;
		}
		headerBuffer[sizeof(headerBuffer) - 1] = 0;
		printf("Can data be recieved from a different thread?: \n %s\n", headerBuffer);

		if (packet::recieveHeaderPacket(headerBuffer, handler->handlerInput.t, handler->handlerInput.p) == 0) 
			continue;
		
		ZeroMemory(headerBuffer, sizeof(headerBuffer));
		
		


		serverSender = packet(SERVER_SENDER, handler->handlerInput.p.serial_number, 0, READ_TABLE, handler->handlerInput.t.getDbName(), handler->handlerInput.t.getTableName(), false);
		
		switch (handler->handlerInput.p.transmition_type)
		{
		case READ_TABLE:

			if (readTable(&(handler->handlerInput.t)) != 1) {
				cout << "error occured with reading\n";
			}

			//Steps:
			//1. Build the data packet so the length will be known.
			//2. Build the header packet and assign the length.
			//3. Send the header packet.
			//4. Send the data packet.
			//*-> Possible logical use of a stack of linked lists/vector of pointers for the sake of the data packet's contents
			//* Will also probably need to refactor

			
			
			cout << "Data packet sent.\n\n";
			
			break;
		case WRITE_TABLE:
			if (writeTable(&(handler->handlerInput.t)) != 1) {
				cout << "error occured with writing\n";
			}
			break;
			// Handle writing a "dirty" table back to the database


		default:
			// Anything other than READ_TABLE OR WRITE_TABLE should not be correct

			break;
		}
		//Once all the functions have finished their work successfully the handler will send the data packet over the network
		// or will send nothing incase a "dirty" page was written back

		
	}

	

	return 10;
}

int setup_handlers(handler_info handlers[], SOCKET serverSock) {

	for (int i = 0; i < MAX_HANDLERS; i++) {
		handlers[i].free = true;
		handlers[i].serverSocket = serverSock;
		
		handlers[i].hHandler = CreateThread(NULL, 0, mainHandler, (void*)&(handlers[i]), CREATE_SUSPENDED, &(handlers[i].threadId));
		if (handlers[i].hHandler == NULL) {
			cout << "Error with thread, potentially no memory, error: " << GetLastError() << '\n';
			return -1;
		}
		
	}
	return 0;
}

int sqliteCallbackRead(void* table, int count, char** data, char** columns) {

	table_info* ptr = (table_info*)table;
	int i;

	size_t totalCols = ptr->getSize();
	for (i = 0; i < count; i++) {
		if (totalCols > 0) {
			(*ptr)[i].setColName(string(columns[i]));
			totalCols--;
		}
		(*ptr)[i].addData(string(data[i]));
		
		


	}
	
	
	return 0;
}


int writeTable(table_info* table) {

	
	sqlite3* db;
	sqlite3_open(table->getDbName().c_str(), &db);
	string sql = "insert or replace into " + table->getTableName() + " values (";

	/*("11", "johnny", 27, "x science"),
		("12", "jerry", 28, "t science"),
		("13", "jimmy", 29, "y science"),
		("14", "terry", 30, "b science"),
		("15", "terry", 31, "s science");*/

	
	int i;
	int j;

	for (i = 0; i < (*table)[0].getSize(); i++) {
		for (j = 0; j < (*table).getSize(); j++) {

			if ((*table)[i].getColType() == "INTEGER")
				sql += (*table)[i][j] + ',';
			else
				sql += '\"' + (*table)[i][j] + '\"' + ',';

				
		}
		sql[sql.size()] = ')';
		if ((size_t)i + 1 == (*table)[0].getSize())
			sql += ';';
		else
			sql += ",\n";
	}

	
	sql += ';';
	char* errMsg = NULL;
	DWORD mtxResult = WaitForSingleObject(writerMutex, INFINITE);
	sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);
	ReleaseMutex(writerMutex);
	if (errMsg != NULL) {
		cout << errMsg;
		sqlite3_free(errMsg);
		return 0;
	}

	return 1;
}



//TODO: Handle the event that the table to be read is larger than 64 KiB
//One way could be to count each byte read from the table and test whether it's larger or can fit, if larger an additional allocation could be required along with an additional send()
//Potentially having a vector of char* for the problem of data larger than 64 KiB, probably max up to 2 char * with 64 KiB heap allocated memory
int readTable(table_info* table) {

	sqlite3* db;
	
	sqlite3_open(table->getDbName().c_str(), &db);
	//cout << "db name is " << table->getDbName() << '\n';
	//cout << "readTable: tablename is " << table->getTableName() << '\n';

	string sqlTableInfo = "pragma table_info ";
	sqlTableInfo += '(' + table->getTableName() + ");";
	//cout << "sql query is " << sqlTableInfo << '\n';

	char* errMsg = NULL;
	
	sqlite3_exec(db, sqlTableInfo.c_str(), sqliteCallbackReadTypes, table, &errMsg);
	//printf("err msg is: %s\n", errMsg);
	if (errMsg != NULL) {
		cout << "err msg not null?\n";
		sqlite3_free(errMsg);
		return 0;
	}
	sqlTableInfo = "select * from " + table->getTableName() + ';';

	sqlite3_exec(db, sqlTableInfo.c_str(), sqliteCallbackRead, table, &errMsg);

	if (errMsg != NULL) {
		cout << "err msg not null?\n";
		sqlite3_free(errMsg);
		return 0;
	}

	sqlite3_close(db);
	return 1;
}

int sqliteCallbackReadTypes(void* table, int count, char** data, char** columns) {

	table_info* ptr = (table_info*)table;
	int i;

	
	for (i = 0; i < count; i++) {
		string typeTest = string(columns[i]);

		if (typeTest == "type") {
			(*ptr).addColumn(Column());
			//cout << data[i] << '\n';
			
			(*ptr)[ptr->getSize()-1].setColType(string(data[i]));
		}
			

	}

	

	return 0;
}

int handlers_scheduler(handler_info handlers[], std::queue<SOCKET>& clientsQueue) {

	int i = 0;
	for (i = 0; i < MAX_HANDLERS; i++) {
		if (handlers[i].free) {
			handlers[i].free = false;
			break;
		}
	}
	
	if (i == MAX_HANDLERS) {
		cout << "is it possible there are no handlers?\n";
		//No available handlers, return 0
		return 0;
	}

	handlers[i].handlerInput.connected_socket = clientsQueue.front();
	clientsQueue.pop();
	

	if (ResumeThread(handlers[i].hHandler) == (DWORD)-1) {
		return 0;
	}

	return 1;
}