#include "table.hpp"

table_info::table_info(string db, string tName, packet headerPacket) {
	db_name = db;
	table_name = tName;
	packets.first = headerPacket;
	sz = 0;
}
table_info::table_info() {
}

void table_info::setTableInfo(string db, string tName) {
	db_name = db;
	table_name = tName;
}
Column& table_info::operator[](size_t index) {
	return cols[index];
}

void table_info::addColumn(Column col) {
	cols.push_back(col);
}

packet& table_info::getHeaderPacket() {
	return packets.first;
}

void table_info::setHeaderPacket(packet& p) {
	packets.first = p;
}

packet& table_info::getBodyPacket()
{
	return packets.second;
}

void table_info::setBodyPacket(packet& p)
{
	packets.second = p;
}

string& table_info::getDbName() {
	return db_name;
}
string& table_info::getTableName() {
	return table_name;
}
void table_info::setDbName(string dbName) {
	db_name = dbName;
}
void table_info::setTableName(string tName) {
	table_name = tName;
}


//---------------------------------------------
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