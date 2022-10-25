#ifndef _COL_H_
#define _COL_H_
#include "utils.hpp"

class Column
{

private:
	string colName;
	string colType;
	vector<string> colData;

public:
	Column(string name, string type);
	Column();
	~Column();
	string& operator[](size_t index);
	void addData(string data);
	string& getColName();
	string& getColType();
	void setColName(string name);
	void setColType(string type);
	size_t getSize();



};



#endif // !_COL_H_

