#ifndef BODY_PACKET_H_
#define BODY_PACKET_H_
#include "Packet.hpp"

class BodyPacket : public Packet
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


#endif // !BODY_PACKET_H_

