#include "table.hpp"

table_info::table_info(string db, string tName, int transmitionType, int serialNum) {
	table_info::database = db;
	table_info::name = tName;
	table_info::serial_number = serialNum;
	table_info::transmition_type = transmitionType;
	sz = 0;
}
table_info::table_info() {
	table_info::serial_number = 0;
	table_info::transmition_type = 0;
	sz = 0;
	
}
table_info::~table_info() {
	cols.clear();
	name.clear();
	database.clear();
	clearBuffers();
	
	
	
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

void table_info::incByteSz() {
	sz++;
}
void table_info::incByteSz(size_t bytes) {
	sz += bytes;
}
size_t table_info::getByteSz() {
	//18,446,744,073,709,551,615
	// 1,152,921,504,606,846,976
	//		   1,099,511,627,776


	return sz;
}

bool table_info::enqueBuffer(size_t bytes) {
	char* toAppend = nullptr;
	try {
		toAppend = new char[bytes];
	}
	catch (std::bad_alloc) {
		return false;

	}
	buffers.push(toAppend);
	return true;
}
void table_info::dequeBuffer() {
	char* tmp = buffers.front();
	buffers.pop();
	delete[] tmp;
	
}
void table_info::clearBuffers() {
	char* tmp = NULL;
	while (!buffers.empty()) {
		tmp = buffers.front();
		buffers.pop();
		delete[] tmp;
	}
	sz = 0;
}
int table_info::getBuffersLength() {
	return buffers.size();
}
char* table_info::getHeadBuffer() {
	return buffers.front();
}
char* table_info::getBackBuffer() {
	return buffers.back();
}
void table_info::clearColumns() {
	cols.clear();
}