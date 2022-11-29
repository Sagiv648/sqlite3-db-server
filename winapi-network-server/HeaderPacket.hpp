#ifndef _HEADERPACKET_H_
#define _HEADERPACKET_H_
#include "packet.hpp"


class HeaderPacket : public Packet 
{
	PacketBlock block;
	


	void buildPacket() override;
	bool recievePacket() override;

};

#endif // !_HeaderPacket_H_

