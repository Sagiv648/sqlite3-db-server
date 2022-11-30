#include "BodyPacket.hpp"



BodyPacket::BodyPacket() 
{
	
	blocks = vector<PacketBlock>();
}

void BodyPacket::buildPacket(Table& tableInfo) {

	string buffer("");
	buffer += "{\r\n";
	size_t i;
	size_t k;

	size_t remainSz = 0;
	for (i = 0; i < tableInfo.getSize(); ++i) {
		buffer += tableInfo[i].getColName() + '|' + tableInfo[i].getColType() + ':';
		for (k = 0; k < tableInfo[i].getSize(); k++) {

			if (k + 1 == tableInfo[i].getSize())
				buffer += tableInfo[i][k] + "\r\n";
			else
				buffer += tableInfo[i][k] + '|';

			

			while ( (remainSz = blocks[blocks.size() - 1].appendBlock(buffer, buffer.size())) > 0) 
				blocks.push_back(PacketBlock());
				
			buffer.clear();
			buffer.shrink_to_fit();


		}

	}

	buffer += '}';
	
	while ((remainSz = blocks[blocks.size() - 1].appendBlock(buffer, buffer.size())) > 0)
		blocks.push_back(PacketBlock());

	buffer.clear();
	buffer.shrink_to_fit();
	
	
}



//TODO: BodyPacket - recievePacket -> Add actual recv functionality to the procedure
bool BodyPacket::recievePacket(SOCKET sender) {


	char buffer[1024]{};
	int recieved = 0;
	int totalLen = 0;
	size_t remainingIndex = 0;
	while ((recieved = recv(sender, buffer, 1024, 0)) > 0) {
		totalLen += recieved;

		remainingIndex = blocks[blocks.size() - 1].appendBlock(string(buffer), recieved);
		cout << "Current body buffer is: \n" << blocks[blocks.size() - 1].getBlock() << '\n';
		if (remainingIndex > 0) {
			blocks.push_back(PacketBlock());
			blocks[blocks.size() - 1].setBlock(string(buffer).substr(remainingIndex, string(buffer).size() - remainingIndex));
		}
		
		memset(buffer, 0, sizeof(buffer));
	}
	if (recieved == SOCKET_ERROR)
		return false;





	/*string buffer("");

	int i = 0;

	for (; i < buffer.size(); ++i) {

		if (buffer[i] == '\n' && buffer[(size_t)i + 1] != '}') {

			string colName = "";
			string colType = "";
			for (++i; i < buffer.size() && buffer[i] != '|'; ++i) {
				colName += buffer[i];
			}
			for (++i; i < buffer.size() && buffer[i] != ':'; ++i) {
				colType += buffer[i];
			}

			tableInfo.addColumn(Column(colName, colType));

			for (; i < buffer.size() && buffer[i] != '\r'; ++i) {
				int k = i + 1;
				string data = "";
				for (; i < buffer.size() && buffer[i] != '|'; ++i) {
					data += buffer[i];
				}
				tableInfo[tableInfo.getSize() - 1].addData(data);
			}
		}
	}*/

	return true;
}

vector<PacketBlock> BodyPacket::getBlocks() {
	return blocks;
}
