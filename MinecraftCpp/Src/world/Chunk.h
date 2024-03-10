#pragma once
#include "Block.h"
#define chunkW 16
#define chunkH 16
#define chunkThickness 16

class Chunk
{
	int x, y, z;
	Block* blocks[chunkH][chunkW][chunkThickness];
};

