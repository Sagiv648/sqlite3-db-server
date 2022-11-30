#ifndef BODY_PACKET_H_
#define BODY_PACKET_H_

#include "PacketBlock.hpp"
#include "Table.hpp"




class BodyPacket
{
	vector<PacketBlock> blocks;
	
public:
	BodyPacket(Table& t);
	BodyPacket();
	void buildPacket(Table& tableInfo);
	bool recievePacket(SOCKET sender);
	
	vector<PacketBlock> getBlocks();

};


#endif // !BODY_PACKET_H_

