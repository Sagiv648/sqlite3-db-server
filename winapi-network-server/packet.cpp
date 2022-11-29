#include "packet.hpp"


Packet::Packet(char opcode, int serialNum, size_t nextPacketLen, char transmitionType, Table& t) {

	blocks = { PacketBlock() };
	op_code = opcode;
	serial_number = serialNum;
	next_packet_length = nextPacketLen;
	transmition_type = transmitionType;
	tableInfo = t;
	
}

Packet::Packet() {

	blocks = { PacketBlock() };
	
	
	op_code = 0;
	serial_number = 0;
	next_packet_length = 0;
	transmition_type = 1;
	
}
void Packet::setPacket(char opcode, int serialNum, size_t nextPacketLen, char transmitionType, Table& t) {

	op_code = opcode;
	serial_number = serialNum;
	next_packet_length = nextPacketLen;
	transmition_type = transmitionType;
	tableInfo = t;
	
}

void Packet::setTable(Table& t) {
	tableInfo = t;
}

vector<PacketBlock> Packet::getBlocks() {
	return blocks;
}

void Packet::setOpCode(char opcode) {
	op_code = opcode;
}
int Packet::getSerial() {
	return serial_number;
}
void Packet::setSerial(int serial) {
	serial_number = serial;
}
size_t Packet::getNextPacketLength() {
	return next_packet_length;
}
void Packet::setNextPacketLength(size_t len) {
	next_packet_length = len;
}
char Packet::getTransmitionType() {
	return transmition_type;
}
void Packet::setTransmitionType(char type) {
	transmition_type = type;
}

//Returns 1 for success and 0 for failure



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
//TODO: Refactor buildDataPacket to include the column type -> TO CHECK



//Actual data packet:
/*
	{\r\n
		(Column_Name_Placeholder_#1)|(Column_Type):(Column_Data_#1)|(Column_Data_#2)|(Column_Data_#N)\r\n
		(Column_Name_Placeholder_#2)|(Column_Type):(Column_Data_#1)|(Column_Data_#2)|(Column_Data_#N)\r\n
		(Column_Name_Placeholder_#N)|(Column_Type):(Column_Data_#1)|(Column_Data_#2)|(Column_Data_#N)\r\n
	}
*/

