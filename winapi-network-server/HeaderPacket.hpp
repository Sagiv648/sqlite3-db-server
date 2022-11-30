#ifndef _HEADERPACKET_H_
#define _HEADERPACKET_H_

#include "PacketBlock.hpp"
#include "Table.hpp"




class HeaderPacket
{
	char op_code;
	int serial_number;
	size_t next_packet_length;
	char transmition_type;
	PacketBlock block;
	

public:
	HeaderPacket(HeaderPacket& header);
	HeaderPacket(char opcode, int serialNum, size_t nextPacketLen, char transmitionType);
	HeaderPacket();
	void buildPacket(Table& tableInfo);
	bool recievePacket(SOCKET sender);
	void setHeader(char opcode, int serialNum, size_t nextPacketLen, char transmitionType);
	char getOpCode();
	void setOpCode(char opcode);
	int getSerial();
	void setSerial(int serial);
	size_t getNextPacketLength();
	void setNextPacketLength(size_t len);
	char getTransmitionType();
	void setTransmitionType(char type);
	PacketBlock getBlock();
	
};

#endif // !_HeaderPacket_H_

