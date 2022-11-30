#ifndef _TABLE_H_
#define _TABLE_H_

#include "Column.hpp"
//#include "HeaderPacket.hpp"
//#include "BodyPacket.hpp"

using std::string;
using std::vector;
//TODO: Refactor table_info class to be managed in a better way and to account for the column types
class Table {

	std::string db_name;
	std::string table_name;
	vector<Column> cols;
	size_t sz;
	std::queue<char*> buffers;

	

	public:
		
		Table(string db,string tName);
		Table();
		Table(Table& t);
		void setTableInfo(string db,string tName);
		Column& operator[](size_t index);
		Table& operator=(Table right);
		void addColumn(Column col);
		string& getDbName();
		string& getTableName();
		void setDbName(string dbName);
		void setTableName(string tName);
		//--------------------------------------
		size_t getSize();
		size_t getBuffersLength();
		void incByteSz();
		void incByteSz(size_t bytes);
		size_t getByteSz();
		bool enqueBuffer(size_t bytes);
		void dequeBuffer();
		void clearBuffers();
		char* getHeadBuffer();
		char* getBackBuffer();
		void clearColumns();
};



#endif