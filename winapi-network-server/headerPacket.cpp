#include "HeaderPacket.hpp"
#include "Packet.hpp"


void HeaderPacket:: buildPacket() {

	string buffer = string("{\r\n");
	

	
	char midBuff[30];
	memset(midBuff, 0, sizeof(midBuff));
	_itoa_s(op_code, midBuff, 10);

	buffer += "Op_Code: " + string(midBuff);

	buffer += "\r\n";
	memset(midBuff, 0, sizeof(midBuff));
	_itoa_s(serial_number, midBuff, 10);
	buffer += "Packet_Serial_Num: " + string(midBuff);
	buffer += "\r\n";
	memset(midBuff, 0, sizeof(midBuff));
	_itoa_s(next_packet_length, midBuff, 10);

	buffer += "Next_Packet_Len: " + string(midBuff);
	buffer += "\r\n";
	memset(midBuff, 0, sizeof(midBuff));
	_itoa_s(transmition_type, midBuff, 10);
	buffer += "Transmition_Type: " + string(midBuff);
	buffer += "\r\n";
	buffer += "Database: " + tableInfo.getDbName();
	buffer += "\r\n";
	buffer += "Table_Name: " + tableInfo.getTableName();
	buffer += "\r\n}";

	block.setBlock(buffer);
}


//TODO: HeaderPacket - recievePacket -> Add actual recv functionality to the procedure
bool HeaderPacket::recievePacket() {






	//string buffer("");

	//int i = 0;
	//vector<string> key_words = vector<string>();
	//key_words.push_back("Op_Code");
	//key_words.push_back("Next_Packet_Len");
	//key_words.push_back("Packet_Serial_Num");
	//key_words.push_back("Transmition_Type");
	//key_words.push_back("Database");
	//key_words.push_back("Table_Name");



	//vector<string> container = vector<string>();
	//for (; i < key_words.size(); i++) {
	//	int j = 0;
	//	int k = 0;
	//	for (; j < buffer.size(); j++) {
	//		if (buffer[j] == key_words[i][0]) {

	//			for (k = 0; k < key_words[i].size() && buffer[j] == key_words[i][k]; k++, j++);

	//			if (k == key_words[i].size()) {

	//				int t = j + 1;
	//				for (; j < buffer.size() && buffer[j] != '\r'; j++);

	//				container.push_back(buffer.substr((size_t)t, (size_t)j - t));

	//			}
	//		}
	//	}
	//}

	//if (container.size() != 6) {
	//	return false;
	//}

	//setPacket(stoi(container[0]), stoi(container[2]), stoi(container[1]), stoi(container[3]),tableInfo);
	//tableInfo.setDbName(container[4]);
	//tableInfo.setTableName(container[5]);
	//


	return true;
}