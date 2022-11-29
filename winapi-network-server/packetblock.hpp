#ifndef _MEMBLOCK_H_
#define _MEMBLOCK_H_
#include "utils.hpp"
#include "network.hpp"

class PacketBlock {

	std::string block;
	size_t sz;
	static const size_t limit = BUFLEN;
public:
	PacketBlock(std::string memblock,size_t memSz);
	PacketBlock();
	std::string getBlock();
	size_t getSize();
	size_t appendBlock(std::string memblock, size_t memSz);
	void setBlock(std::string buffer);
	



};



#endif // !_MEMBLOCK_H_1

