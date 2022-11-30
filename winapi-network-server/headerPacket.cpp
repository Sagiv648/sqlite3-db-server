#include "HeaderPacket.hpp"



HeaderPacket::HeaderPacket() {

}


HeaderPacket::HeaderPacket(HeaderPacket& header) : HeaderPacket(header.op_code,header.serial_number,header.next_packet_length,header.transmition_type)
{
	
}

HeaderPacket::HeaderPacket(char opcode, int serialNum, size_t nextPacketLen, char transmitionType)
{
	
	op_code = opcode;
	serial_number = serialNum;
	next_packet_length = nextPacketLen;
	transmition_type = transmitionType;
	
}


void HeaderPacket:: buildPacket(Table& tableInfo) {

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
bool HeaderPacket::recievePacket(SOCKET sender) {

	char buffer[1024]{};
	int recieved = 0;
	int totalLen = 0;
	size_t remainingIndex = 0;



	//TOOD: Refactor to account for only one block for header
	/*while ((recieved = recv(sender, buffer, 1024, 0)) > 0) {
		totalLen += recieved;
		
		remainingIndex = blocks[blocks.size() - 1].appendBlock(string(buffer), recieved);
		cout << "Current header buffer is: \n" << blocks[blocks.size() - 1].getBlock() << '\n';
		if (remainingIndex > 0) {
			blocks.push_back(PacketBlock());
			blocks[blocks.size() - 1].setBlock(string(buffer).substr(remainingIndex, string(buffer).size() - remainingIndex));
		}
		
		memset(buffer, 0, sizeof(buffer));
	}
	if (recieved == SOCKET_ERROR)
		return false;*/




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

void HeaderPacket::setHeader(char opcode, int serialNum, size_t nextPacketLen, char transmitionType) {

	op_code = opcode;
	serial_number = serialNum;
	next_packet_length = nextPacketLen;
	transmition_type = transmitionType;
	

}



PacketBlock HeaderPacket::getBlock() {
	return block;
}

char HeaderPacket::getOpCode() {
	return op_code;
}

void HeaderPacket::setOpCode(char opcode) {
	op_code = opcode;
}
int HeaderPacket::getSerial() {
	return serial_number;
}
void HeaderPacket::setSerial(int serial) {
	serial_number = serial;
}
size_t HeaderPacket::getNextPacketLength() {
	return next_packet_length;
}
void HeaderPacket::setNextPacketLength(size_t len) {
	next_packet_length = len;
}
char HeaderPacket::getTransmitionType() {
	return transmition_type;
}
void HeaderPacket::setTransmitionType(char type) {
	transmition_type = type;
}
