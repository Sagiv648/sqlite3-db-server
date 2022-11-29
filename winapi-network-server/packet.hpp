#ifndef _PACKET_H_
#define _PACKET_H_
#include "network.hpp"
#include "utils.hpp"
#include "table.hpp"
#include "packetblock.hpp"
#include <list>
//Packet header:
/*
	{\r\n
		Op_Code: %(numeric->constant)\r\n
		Packet_Serial_Num: %(numeric)\r\n
		Next_Packet_Len: %(numeric)\r\n
		Transmition_Type: %(numeric->constant)\r\n
		Table_Name: %(string)\r\n
	}
*/


//Actual data packet:
/*
	{\r\n
		%(Column_Name_Placeholder_#1):(%(Column_Data_#1)|%(Column_Data_#2)|%(Column_Data_#N))\r\n
		%(Column_Name_Placeholder_#2):(%(Column_Data_#1)|%(Column_Data_#2)|%(Column_Data_#N))\r\n
		%(Column_Name_Placeholder_#N):(%(Column_Data_#1)|%(Column_Data_#2)|%(Column_Data_#N))\r\n
	}
*/


class Packet
{
protected:
	char op_code;
	int serial_number;
	size_t next_packet_length;
	char transmition_type;
	Table tableInfo;
	vector<PacketBlock> blocks;

	public:
		virtual void buildPacket();
		virtual bool recievePacket();
	Packet(char opcode, int serialNum, size_t nextPacketLen, char transmitionType,Table& t); //^
	Packet();
	
	 void setPacket(char opcode, int serialNum, size_t nextPacketLen, char transmitionType,Table& t);
	 void setTable(Table& t);
};

#endif // !_PACKET_H_H


