#pragma once
#include "Block.h"
#include "StructureHandler.h"
#define StructureTileSize 15

Block* createBlock(int i, int x, int y, int z);

StructureHandler* createStructure(int i, int x, int y, int z);

void changeStructureVariant(StructureHandler* str,int i);