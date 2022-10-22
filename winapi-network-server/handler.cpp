#include "handler.hpp"



HANDLE writerMutex;

DWORD mainHandler(void* handlerInput) {

	
	handler_info* handler = (handler_info*)handlerInput;
	cout << "this is thread with id " << handler->threadId << "\n\n\n";
	
	// packet class and table_info class are initialized by the recieveHeaderPacket function
	while (true)
	{
		
		char* buffer = new char[BUFLEN];
		ZeroMemory(buffer, BUFLEN);
		switch (handler->handlerInput.p.transmition_type)
		{
		case READ_TABLE:
			if (readTable(&(handler->handlerInput.t)) != 1) {
				cout << "error occured with reading\n";
			}
			handler->handlerInput.p.buildDataPacket(buffer, handler->handlerInput.t);
			cout << "Data packet is:\n\n";
			printf("%s\n", buffer);
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

		handler->free = true;
		delete[] buffer;
		SuspendThread(handler->hHandler);
		cout << "thread was suspended but continued here\n";
		
	}

	

	return 10;
}

int setup_handlers(handler_info handlers[], SOCKET serverSock) {

	for (int i = 0; i < MAX_HANDLERS; i++) {
		handlers[i].free = true;
		handlers[i].socket = serverSock;
		
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


	for (int i = 0; i < count; i++) {
		if (ptr->columns.size() != count) {
			ptr->columns.push_back(columns[i]);
			ptr->data.push_back(vector<string>());
		}
		ptr->data[i].push_back(data[i]);
	}
	
	return 0;
}


int writeTable(table_info* table) {

	
	sqlite3* db;
	sqlite3_open(table->database.c_str(), &db);
	string sql = "insert or replace into " + table->name + " values (";

	/*("11", "johnny", 27, "x science"),
		("12", "jerry", 28, "t science"),
		("13", "jimmy", 29, "y science"),
		("14", "terry", 30, "b science"),
		("15", "terry", 31, "s science");*/

	
	int i;
	int j;
	for (i = 0; i < table->data[0].size(); ++i) {
		for (j = 0; j < table->data.size(); ++j) {
			if ((size_t)j + 1 == table->data.size()) {

				sql += '\"' + table->data[j][i] + '\"' + ')';
			}
			else {
				if (j == 2) {
					sql += table->data[j][i] + ',';
					continue;
				}
				sql += '\"' + table->data[j][i] + '\"' + ',';
			}
		}
		if ((size_t)i + 1 != table->data[0].size()) {
			sql += ',';
			sql += "\n(";
		}
		
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
int readTable(table_info* table) {

	sqlite3* db;
	
	sqlite3_open(table->database.c_str(), &db);
	cout << "readTable: tablename is " << table->name << '\n';
	string sql = "select * from " + table->name + ';';
	char* errMsg = NULL;
	
	sqlite3_exec(db, sql.c_str(), sqliteCallbackRead, table, &errMsg);
	
	if (errMsg != NULL) {
		sqlite3_free(errMsg);
		return 0;
	}

	/*cout << "\n\n\n";
	for (int i = 0; i < table->columns.size(); ++i) {
		cout << table->columns[i] << ": ";
		for (int j = 0; j < table->data[i].size(); ++j) {

			cout << table->data[i][j] << ",";
		}
		cout << "\n\n";
	}*/
	sqlite3_close(db);
	return 1;
}

int handlers_scheduler(handler_info handlers[], char* packetBuffer) {

	int i = 0;
	for (i = 0; i < MAX_HANDLERS; i++) {
		if (handlers[i].free) {
			handlers[i].free = false;
		}
	}
	if (i == MAX_HANDLERS) {
		//No available handlers, return 0
		return 0;
	}


	if (packet::recieveHeaderPacket(packetBuffer, handlers[i].handlerInput.t, handlers[i].handlerInput.p) == 0) {
		cout << "incorrect format\n";
		return 0;
	}
	else {

		if (ResumeThread(handlers[i].hHandler) == (DWORD)-1) {
			return 0;
		}

	}
	return 1;
}