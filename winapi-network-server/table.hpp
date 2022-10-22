#ifndef _TABLE_H_
#define _TABLE_H_
#include "utils.hpp"
#include "network.hpp"



//TODO: Refactor table_info class to be managed in a better way and to account for the column types
class table_info {
public:
	std::string database;
	std::string name;
	int transmition_type;
	int serial_number;
	vector<std::string> columns;
	vector<vector<string>> data;

	public:
		table_info(string db,string tName, int transmitionType, int serialNum);
		table_info();
		~table_info();
		void setTableInfo(string db,string tName, int transmitionType, int serialNum);

};



#endif