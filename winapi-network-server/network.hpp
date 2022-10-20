#ifndef _NETWORK_H_
#define _NETWORK_H_
#include "utils.hpp"

//Packet codes for operations identification

//Op codes
#define SERVER_SENDER 100
#define CLIENT_SENDER 200
#define CLIENT_DISCONNECT -200

//Transmition types
#define READ_TABLE 1
#define WRITE_TABLE 2

//TODO: Probably compact all the packet characteristics into a single struct/class
char* sendPacket(int sender, int file_mod, size_t fileSz, string fileName);

/*Since the application is mainly to have a client computer access a db from a server through the network
the client will get the tables from the server through the network in an encrypted fashion which the client app will have the means to decrypt
and send encrypted "dirty" tables back to the server
*/

// Packet headers will have serial numbers to determine their relation (Ex. a header packet of an ongoing data will contain the same serial number as the next ongoing data packet, along with it's length)

// Incoming client packets will also have transmition types, with generally 3 modes: 1) request of a table | 2) writing of a "dirty" table | 3) disconnection which will leave all fields empty except of the right op_code

// Since packets are enclosed with {} a packet will begin with { and end with }


//Packet header:
/*
	{\r\n
		Op_Code:%(numeric->constant)\r\n
		Next_Packet_Len:%(numeric)\r\n
		Packet_Serial_Num:%(numeric)\r\n
		Transmition_Type:%(numeric->constant)\r\n
		Database:%(string)\r\n
		Table_Name:%(string)\r\n
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

void recvPacket(char* buffer);

#endif // !_NETWORK_H_
