#ifndef _TABLE_H_
#define _TABLE_H_
#include "utils.hpp"
#include "network.hpp"
#include "column.hpp"
#include "packet.hpp"


//TODO: Refactor table_info class to be managed in a better way and to account for the column types
class table_info {

	std::string db_name;
	std::string table_name;
	vector<Column> cols;
	size_t sz;
	std::queue<char*> buffers;
	
	std::pair<packet, packet> packets;

	public:
		
		table_info(string db,string tName, packet table_packet);
		table_info();
		
		void setTableInfo(string db,string tName);
		Column& operator[](size_t index);
		void addColumn(Column col);
		string& getDbName();
		string& getTableName();
		packet& getHeaderPacket();
		void setHeaderPacket(packet& p);
		packet& getBodyPacket();
		void setBodyPacket(packet& p);
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