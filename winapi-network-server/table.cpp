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
	cols.clear();
	name.clear();
	database.clear();
	
	
	
}
void table_info::setTableInfo(string db, string tName, int transmitionType, int serialNum) {
	table_info::database = db;
	table_info::name = tName;
	table_info::serial_number = serialNum;
	table_info::transmition_type = transmitionType;
}
Column& table_info::operator[](size_t index) {
	return cols[index];
}

void table_info::addColumn(Column col) {
	cols.push_back(col);
}

string& table_info::getDbName() {
	return database;
}
string& table_info::getTableName() {
	return name;
}
int& table_info::getTransmitionType() {
	return transmition_type;
}
int& table_info::getSerialNum() {
	return serial_number;
}
void table_info::setDbName(string dbName) {
	database = dbName;
}
void table_info::setTableName(string tName) {
	name = tName;
}
void table_info::setTransmitionType(int transmitType) {
	transmition_type = transmitType;
}
void table_info::setSerialNum(int serialNum) {
	serial_number = serialNum;
}
size_t table_info::getSize() {

	return cols.size();
}

bool table_info::addBuffer(size_t bytes) {
	char* toAppend = nullptr;
	try {
		toAppend = new char[bytes];
	}
	catch (std::bad_alloc) {
		return false;

	}
	buffers.push_back(toAppend);
	return true;
}
void table_info::removeBuffer() {
	delete[] buffers.back();
	buffers.pop_back();
}