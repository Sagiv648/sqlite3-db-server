#include "table.hpp"

Table::Table(string db, string tName, HeaderPacket header) {
	db_name = db;
	table_name = tName;
	packets.first = header;
	sz = 0;
}
Table::Table() {
}

void Table::setTableInfo(string db, string tName) {
	db_name = db;
	table_name = tName;
}
Column& Table::operator[](size_t index) {
	return cols[index];
}

void Table::addColumn(Column col) {
	cols.push_back(col);
}

HeaderPacket& Table::getHeaderPacket() {
	return packets.first;
}

void Table::setHeaderPacket(HeaderPacket& p) {
	packets.first = p;
}

BodyPacket& Table::getBodyPacket()
{
	return packets.second;
}

void Table::setBodyPacket(BodyPacket& p)
{
	packets.second = p;
}

string& Table::getDbName() {
	return db_name;
}
string& Table::getTableName() {
	return table_name;
}
void Table::setDbName(string dbName) {
	db_name = dbName;
}
void Table::setTableName(string tName) {
	table_name = tName;
}


//---------------------------------------------
size_t Table::getSize() {
	
	return cols.size();
}

void Table::incByteSz() {
	sz++;
}
void Table::incByteSz(size_t bytes) {
	sz += bytes;
}
size_t Table::getByteSz() {
	//18,446,744,073,709,551,615
	// 1,152,921,504,606,846,976
	//		   1,099,511,627,776


	return sz;
}

bool Table::enqueBuffer(size_t bytes) {
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
void Table::dequeBuffer() {
	char* tmp = buffers.front();
	buffers.pop();
	delete[] tmp;
	
}
void Table::clearBuffers() {
	char* tmp = NULL;
	while (!buffers.empty()) {
		tmp = buffers.front();
		buffers.pop();
		delete[] tmp;
	}
	sz = 0;
}
int Table::getBuffersLength() {
	return buffers.size();
}
char* Table::getHeadBuffer() {
	return buffers.front();
}
char* Table::getBackBuffer() {
	return buffers.back();
}
void Table::clearColumns() {
	cols.clear();
}