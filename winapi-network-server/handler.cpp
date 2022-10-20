#include "handler.hpp"



DWORD mainHandler(void* handlerInput) {

	
	handler_info* handler = (handler_info*)handlerInput;
	cout << "this is thread with id " << handler->threadId << "\n\n\n";
	// packet class and table_info class are initialized by the recieveHeaderPacket function
	switch (handler->handlerInput.p.transmition_type)
	{
		case READ_TABLE:
			if (readTable(&(handler->handlerInput.t)) != 1) {
					cout << "error occured\n";
				}
	
				char buffer[200];
				ZeroMemory(buffer, sizeof(buffer));
				handler->handlerInput.p.buildDataPacket(buffer, handler->handlerInput.t);
				cout << "Data packet is:\n\n";
				printf("%s\n", buffer);
				break;
		case WRITE_TABLE:
			
			// Handle writing a "dirty" table back to the database


	default:
		// Anything other than READ_TABLE OR WRITE_TABLE should not be correct

		break;
	}
	
	
	

	//Once all the functions have finished their work successfully the handler will send the data packet over the network
	// or will send nothing incase a "dirty" page was written back

	return 10;
}

int setup_handlers(handler_info handlers[], SOCKET serverSock) {

	for (int i = 0; i < MAX_HANDLERS; i++) {
		handlers[i].socket = serverSock;
		handlers[i].hHandler = CreateThread(NULL, 0, mainHandler, (void*)&(handlers[i]), CREATE_SUSPENDED, &(handlers[i].threadId));
		if (handlers[i].hHandler == NULL) {
			cout << "Error with thread, potentially no memory, error: " << GetLastError() << '\n';
			return -1;
		}

	}
	return 0;
}

int sqliteCallback(void* table, int count, char** data, char** columns) {

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

int readTable(table_info* table) {

	sqlite3* db;
	
	sqlite3_open(table->database.c_str(), &db);
	//cout << "readTable: tablename is " << table->name << '\n';
	string sql = "select * from " + table->name + ';';
	char* errMsg = NULL;
	
	sqlite3_exec(db, sql.c_str(), sqliteCallback, table, &errMsg);
	
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