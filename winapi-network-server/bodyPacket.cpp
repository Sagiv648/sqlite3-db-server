#include "BodyPacket.hpp"


void BodyPacket::buildPacket() {

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
bool BodyPacket::recievePacket() {









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