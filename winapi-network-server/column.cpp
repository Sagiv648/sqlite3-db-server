#include "Column.hpp"



Column::Column(string name, string type) {
	colName = name;
	colType = type;
}
Column::Column() {
	colName = "";
	colType = "";

}
Column::~Column() {
	colName.clear();
	colType.clear();
	colData.clear();
}
string& Column::operator[](size_t index) {

	return colData[index];
}
void Column::addData(string data) {
	
	colData.push_back(data);
	
}
string& Column::getColName() {
	return colName;
}
string& Column::getColType() {
	return colType;
}
void Column::setColName(string name) {
	colName = name;
}
void Column::setColType(string type) {
	colType = type;
}
size_t Column::getSize() {
	return colData.size();
}