#include "packetblock.hpp"



PacketBlock::PacketBlock(std::string memblock, size_t memSz) {
	std::string block = memblock;
	sz = memSz;
	

}

PacketBlock::PacketBlock() : PacketBlock("", 0) {}


size_t PacketBlock::getSize() {
	return sz;
}

size_t PacketBlock::appendBlock(std::string memblock, size_t memSz)
{
	if (sz + memSz >= limit) {
		block += memblock.substr(0, limit - sz);
		sz = limit;
		return memSz - (limit - sz);
	}
	block += memblock;
	sz += memSz;
	return 0;
	
}

std::string PacketBlock::getBlock() {
	return block;
}

