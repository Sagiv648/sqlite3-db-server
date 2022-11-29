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
		virtual bool recievePacket(SOCKET sender);
	Packet(char opcode, int serialNum, size_t nextPacketLen, char transmitionType,Table& t); //^
	Packet();
	
	 virtual void setPacket(char opcode, int serialNum, size_t nextPacketLen, char transmitionType,Table& t);
	 virtual void setTable(Table& t);
	 virtual char getOpCode();
	 virtual void setOpCode(char opcode);
	 virtual int getSerial();
	 virtual void setSerial(int serial);
	 virtual size_t getNextPacketLength();
	 virtual void setNextPacketLength(size_t len);
	 virtual char getTransmitionType();
	 virtual void setTransmitionType(char type);
	 virtual vector<PacketBlock> getBlocks();

	 
};

#endif // !_PACKET_H_H


