#pragma once
#include "Block.h"
#include "StructureHalder.h"
#define StructureTileSize 15

Block* createBlock(int i, int x, int y, int z);

StructureHalder* createStructure(int i, int x, int y, int z);