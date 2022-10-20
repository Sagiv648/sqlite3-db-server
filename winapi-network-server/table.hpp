#ifndef _TABLE_H_
#define _TABLE_H_
#include "utils.hpp"
#include "network.hpp"

//1) packet_serial_num, 2) next_packet_len, 3) transmition_type, 4) table_name
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