#ifndef _TABLE_H_
#define _TABLE_H_
#include "utils.hpp"
#include "network.hpp"
#include "column.hpp"
#include "packet.hpp"
#include "HeaderPacket.hpp"
#include "BodyPacket.hpp"

//TODO: Refactor table_info class to be managed in a better way and to account for the column types
class Table {

	std::string db_name;
	std::string table_name;
	vector<Column> cols;
	size_t sz;
	std::queue<char*> buffers;
	
	std::pair<HeaderPacket, BodyPacket> packets;

	public:
		
		Table(string db,string tName, HeaderPacket header);
		Table();
		
		void setTableInfo(string db,string tName);
		Column& operator[](size_t index);
		void addColumn(Column col);
		string& getDbName();
		string& getTableName();
		HeaderPacket& getHeaderPacket();
		void setHeaderPacket(HeaderPacket& p);
		BodyPacket& getBodyPacket();
		void setBodyPacket(BodyPacket& p);
		void setDbName(string dbName);
		void setTableName(string tName);
		//--------------------------------------
		size_t getSize();
		int getBuffersLength();
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