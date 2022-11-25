
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



void parseArguments(char** args, int count, EntryArguments* pEntryArgs) {

	// -p 7777 -addr 127.0.0.1

	vector<string> vecArgs = vector<string>();
	int i = 1;
	for (; i < count; i++) {
		vecArgs.push_back(args[i]);
	}
	if (vecArgs.size() < 2 || vecArgs.size() > 4 || vecArgs[0] != "-p" || !isNumeric(vecArgs[1])) {
		std::cout << "Mandatory parameter is -p(port) and has to be numeric, -addr will default to 0.0.0.0\n";
		ExitProcess(0);
	}
	stringstream ss = stringstream();
	unsigned long testNumeric = 0;
	ss << vecArgs[1];
	ss >> testNumeric;
	if (testNumeric < 1024 || testNumeric > UINT16_MAX) {
		cout << "Invalid port number " << testNumeric << '\n';
		ExitProcess(0);
	}
	pEntryArgs->port = (unsigned short)testNumeric;
	if (vecArgs.size() == 2)
	{
		pEntryArgs->isAutomatic = true;
		pEntryArgs->ipv4 = 0;
	}
	else if(vecArgs.size() == 4) {
		pEntryArgs->isAutomatic = false;
		getIpv4(vecArgs[3], &(pEntryArgs->ipv4));
		if (pEntryArgs->ipv4 == ULONG_MAX)
		{
			cout << "Invalid address " << vecArgs[3] << '\n';
			ExitProcess(0);
		}
	}
	
}

bool isNumeric(string str) {

	for (auto c : str) {
		if (c < '0' || c > '9')
			return false;
	}
	return true;
}

void getIpv4(string ipv4, unsigned long* outIp) {

	vector<unsigned long> ip_addr = vector<unsigned long>();
	char* nextToken = NULL;
	char* ptr = strtok_s((char*)ipv4.c_str(), ".", &nextToken);
	stringstream ss = stringstream();
	unsigned long test = 0;
	while (ptr) {
		
		
		if (!isNumeric(string(ptr))) {
			*outIp = ULONG_MAX;
			return;

		}
		ss << string(ptr);
		ss >> test;
		ss.clear();
		
		if (test < 0 || test > UCHAR_MAX)
		{
			*outIp = ULONG_MAX;
			return;
		}
		ip_addr.push_back(test);
		test = 0;
		
		ptr = strtok_s(NULL, ".", &nextToken);
	
	}
	
	*outIp = getDecimalIp(ip_addr);

	
}

unsigned long getDecimalIp(vector <unsigned long> ipAddr) {
	
	
	if (ipAddr.size() < 4)
		return ULONG_MAX;
		

	unsigned long long output = 0;
	unsigned char offset = 8;

	for (auto uc : ipAddr) {
		output = output << offset;
		output |= (unsigned long long)uc;
	}

	return (unsigned long)output;
}
