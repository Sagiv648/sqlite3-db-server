#include "table.hpp"

table_info::table_info(string db, string tName, int transmitionType, int serialNum) {
	table_info::database = db;
	table_info::name = tName;
	table_info::serial_number = serialNum;
	table_info::transmition_type = transmitionType;
}
table_info::table_info() {
	table_info::serial_number = 0;
	table_info::transmition_type = 0;
}
table_info::~table_info() {
	table_info::columns.clear();
	table_info::data.clear();
	table_info::name.clear();
	table_info::database.clear();
	
}
void table_info::setTableInfo(string db, string tName, int transmitionType, int serialNum) {
	table_info::database = db;
	table_info::name = tName;
	table_info::serial_number = serialNum;
	table_info::transmition_type = transmitionType;
}