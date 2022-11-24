#ifndef _UTILS_H_
#define _UTILS_H_


#define CAP_FILE_READ 101

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::cout;
using std::stringstream;


string readFromFile();

std::map<string,string> parseFromFile(string);


string numToString(long long num);
int stringToNum(string str);


typedef struct EntryArguments {
	unsigned short port;
	bool isAutomatic;
	unsigned long ipv4;
} EntryArguments;





void parseArguments(char** args, int count, EntryArguments* pEntryArgs);

bool isNumeric(string str);

void getIpv4(string ipv4, unsigned long* outIp);

unsigned long getDecimalIp(vector <unsigned char> ipAddr);

#endif // !_UTILS_H_
