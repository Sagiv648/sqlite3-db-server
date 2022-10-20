
#include "utils.hpp"



string readFromFile() {

	

	OFSTRUCT fileStruct;
	ZeroMemory(&fileStruct, sizeof(fileStruct));

	
	
	HANDLE file = CreateFileA("../../cpp-project-config.env", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		return "";
	}
	
	DWORD fSize = GetFileSize(file, NULL);
	
	char* buffer = new char[(size_t)fSize+1];
	
	unsigned long size = 0;

	if ((ReadFile(file, buffer, fSize, &size, NULL)) == FALSE) {
		delete[] buffer;
		return "";
	}
	buffer[fSize] = 0;
	CloseHandle(file);
	return buffer;
}

std::map<string,string> parseFromFile(string details) {

	vector<string> list = vector<string>();
	std::map<string, string> output = std::map<string, string>();
	size_t i=0;
	
	if (details[0] == '#') {
		for (; i < details.size() && details[i] != '\n'; ++i);
	}
	for (; i < details.size(); ++i) {
		if (details[i] == '#') {
			for (; i < details.size() && details[i] != '\n'; ++i);
		}
		string buffer = "";
		
		for (; i < details.size() && details[i] != '\n'; ++i) {
			buffer += details[i];
		}
		if (buffer.size() > 0) {
			buffer = buffer.substr(0, buffer.size()-2);
			list.push_back(buffer);
		}
	
	}
	
	size_t j;
	for (i = 0; i < list.size(); i++) {
		
		for (j = 0; j < list[i].size(); j++) {
			if (list[i][j] == '=') {
				output.insert({ {list[i].substr(0,j),
								list[i].substr(j + 1,list[i].size() - 1) } });
			}
		}
	}


	return output;
}

string numToString(long long num) {

	string output = "";
	long long numBuffer = num;
	if (num < 0) {
		output += '-';
		numBuffer *= -1;
		num *= -1;
	}

	char counter = 0;
	while (numBuffer) {
		counter++;
		numBuffer /= 10;
	}

	counter--;
	while (counter >= 0) {

		output += (((num / ((int)pow(10, counter))) % 10) + '0');
		counter--;
	}

	return output;
}

int stringToNum(string str) {

	int output = 0;
	size_t i = 0;
	if (str[0] == '-') i++;
	for (; i < str[i]; ++i) {
		if (str[i] < '0' || str[i] > '9') return 0;
		output *= 10;
		output += str[i] - (size_t)'0';

	}
	if (str[0] == '-')
		return ~output + 1;

	return output;
}