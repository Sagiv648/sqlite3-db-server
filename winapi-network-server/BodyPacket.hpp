#ifndef BODY_PACKET_H_
#define BODY_PACKET_H_
#include "Packet.hpp"

class BodyPacket : public Packet
{
	std::vector<PacketBlock> blocks;

	void buildPacket() override;
	bool recievePacket() override;


};


#endif // !BODY_PACKET_H_

