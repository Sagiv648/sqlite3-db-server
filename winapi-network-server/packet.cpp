#include "packet.hpp"


packet::packet(char opcode, int serialNum, size_t nextPacketLen, char transmitionType, string dbName, string tableName, bool isDataPacket) {

	packet::op_code = opcode;
	packet::serial_number = serialNum;
	packet::next_packet_length = nextPacketLen;
	packet::transmition_type = transmitionType;
	packet::db_name = dbName;
	packet::table_name = tableName;
	packet::is_Data_Packet = isDataPacket;
}
packet::~packet() {

}
packet::packet() {
	packet::op_code = 0;
	packet::serial_number = 0;
	packet::next_packet_length = 0;
	packet::transmition_type = 1;
	packet::db_name = "";
	packet::table_name = "";
	packet::is_Data_Packet = false;
}
void packet::setPacket(char opcode, int serialNum, size_t nextPacketLen, char transmitionType, string dbName, string tableName, bool isDataPacket) {

	packet::op_code = opcode;
	packet::serial_number = serialNum;
	packet::next_packet_length = nextPacketLen;
	packet::transmition_type = transmitionType;
	packet::db_name = dbName;
	packet::table_name = tableName;
	packet::is_Data_Packet = isDataPacket;
}

void packet::buildHeaderPacket(char* packetBuffer) {

	string buffer = string("");
	buffer += "{\r\n";
	buffer += "Op_Code: " + numToString( packet::op_code);
	buffer += "\r\n";
	buffer += "Packet_Serial_Num: " + numToString( packet::serial_number);
	buffer += "\r\n";
	buffer += "Next_Packet_Len: " + numToString( packet::next_packet_length);
	buffer += "\r\n";
	buffer += "Transmition_Type: " + numToString( packet::transmition_type);
	buffer += "\r\n";
	buffer += "Database: " + packet::db_name;
	buffer += "\r\n}";
	buffer += "Table_Name: " + packet::table_name;
	buffer += "\r\n}";
	const char* tmp = buffer.c_str();
	memcpy(packetBuffer, tmp, strlen(tmp));
	packetBuffer[buffer.size()] = 0;
	
}

//Returns 1 for success and 0 for failure
int packet::recieveHeaderPacket(char* packetBuffer, table_info& tInfo, packet& p) {
	
	

	string buffer(packetBuffer);
	
	int i = 0;
	vector<string> key_words = vector<string>();
	key_words.push_back("Op_Code");
	key_words.push_back("Next_Packet_Len");
	key_words.push_back("Packet_Serial_Num");
	key_words.push_back("Transmition_Type");
	key_words.push_back("Database");
	key_words.push_back("Table_Name");
	
	

	vector<string> container = vector<string>();
	for (; i < key_words.size(); i++) {
		int j = 0;
		int k = 0;
		for (; j < buffer.size();j++) {
			if (buffer[j] == key_words[i][0]) {
				
				for (k = 0; k < key_words[i].size() && buffer[j] == key_words[i][k]; k++, j++);

				if (k == key_words[i].size()) {
					
					int t = j+1;
					for (; j < buffer.size() && buffer[j] != '\r'; j++);
				
					container.push_back(buffer.substr((size_t)t, (size_t)j - t));
					
				}
			}
		}
	}
	
	if (container.size() != 6) {
		return 0;
	}

	p = packet(stoi(container[0]), stoi(container[2]), stoi(container[1]), stoi(container[3]), container[4], container[5], false);
	tInfo = table_info(container[4], container[5], stoi(container[3]), stoi(container[2]));
	return 1;
	
}



//Packet header:
/*
	{\r\n
		Op_Code:%(numeric->constant)\r\n
		Next_Packet_Len:%(numeric)\r\n
		Packet_Serial_Num:%(numeric)\r\n
		Transmition_Type:%(numeric->constant)\r\n
		Database:%(string)\r\n
		Table_Name:%(string)\r\n
	}
*/


//According to sqlite3, the base storage classes are:
/*
	NULL. The value is a NULL value.

	INTEGER. The value is a signed integer, stored in 0, 1, 2, 3, 4, 6, or 8 bytes depending on the magnitude of the value.

	REAL. The value is a floating point value, stored as an 8-byte IEEE floating point number.

	TEXT. The value is a text string, stored using the database encoding (UTF-8, UTF-16BE or UTF-16LE).

	BLOB. The value is a blob of data, stored exactly as it was input.

*/

//Actual data packet:
/*
	{\r\n
		(Column_Name_Placeholder_#1)|(Column_Type):(Column_Data_#1)|(Column_Data_#2)|(Column_Data_#N)\r\n
		(Column_Name_Placeholder_#2)|(Column_Type):(Column_Data_#1)|(Column_Data_#2)|(Column_Data_#N)\r\n
		(Column_Name_Placeholder_#N)|(Column_Type):(Column_Data_#1)|(Column_Data_#2)|(Column_Data_#N)\r\n
	}
*/
//TODO: Refactor buildDataPacket to include the column type
void packet::buildDataPacket(char* packetBuffer, table_info& tInfo) {

	string buffer("");
	buffer += "{\r\n";
	size_t i;
	size_t k;
	for (i = 0; i < tInfo.columns.size(); ++i) {
		buffer += tInfo.columns[i] + ':';
		
		for (k = 0; k < tInfo.data[i].size(); k++) {
			if(k + 1 == tInfo.data[i].size())
				buffer += tInfo.data[i][k] + "\r\n";
			else
			buffer += tInfo.data[i][k] + '|';
		}
	}
	buffer += '}';
	
	const char* tmp = buffer.c_str();
	memcpy(packetBuffer, tmp, strlen(tmp));
	packetBuffer[buffer.size()] = 0;

}
//TODO: Refactor recieveDataPacket to include the column type
void packet::recieveDataPacket(char* packetBuffer, table_info& tInfo) {
	string buffer(packetBuffer);

	int i = 0;

	
	for (; i < buffer.size(); ++i) {
		if (buffer[i] == '\n' && buffer[(size_t)i+1] != '}') {
			int k = i + 1;
			for (; i < buffer.size() && buffer[i] != ':'; ++i);
			tInfo.columns.push_back(buffer.substr(k, (size_t)i - k));
	
			// At this point buffer[i] is at the ':'
			tInfo.data.push_back(vector<string>());

			int t = i + 1;
			for (; i < buffer.size() && buffer[i] != '\r'; ++i) {
				if (buffer[i] == '|') {
					tInfo.data[tInfo.columns.size() - 1].push_back(buffer.substr(t, (size_t)i - t));
					t = i + 1;
				}
			}
			// At this point buffer[i] is at the '\r'
			tInfo.data[tInfo.columns.size() - 1].push_back(buffer.substr(t, (size_t)i - t));


		}
	}
}