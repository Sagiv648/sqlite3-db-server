#ifndef _HEADERPACKET_H_
#define _HEADERPACKET_H_
#include "packet.hpp"


class HeaderPacket : public Packet 
{
	
	

public:
	void buildPacket() override;
	bool recievePacket(SOCKET sender) override;
	void setPacket(char opcode, int serialNum, size_t nextPacketLen, char transmitionType, Table& t);
	void setTable(Table& t);
	char getOpCode();
	void setOpCode(char opcode);
	int getSerial();
	void setSerial(int serial);
	size_t getNextPacketLength();
	void setNextPacketLength(size_t len);
	char getTransmitionType();
	void setTransmitionType(char type);
	vector<PacketBlock> getBlocks();
	
};

#endif // !_HeaderPacket_H_

