#ifndef _TABLE_H_
#define _TABLE_H_
#include "utils.hpp"
#include "network.hpp"
#include "column.hpp"



//TODO: Refactor table_info class to be managed in a better way and to account for the column types
class table_info {
private:
	std::string database;
	std::string name;
	int transmition_type;
	int serial_number;
	vector<Column> cols;
	

	public:
		vector<char*> buffers;
		table_info(string db,string tName, int transmitionType, int serialNum);
		table_info();
		~table_info();
		void setTableInfo(string db,string tName, int transmitionType, int serialNum);
		Column& operator[](size_t index);
		void addColumn(Column col);
		string& getDbName();
		string& getTableName();
		int& getTransmitionType();
		int& getSerialNum();
		void setDbName(string dbName);
		void setTableName(string tName);
		void setTransmitionType(int transmitType);
		void setSerialNum(int serialNum);
		size_t getSize();

		bool addBuffer(size_t bytes);
		void removeBuffer();

};



#endif