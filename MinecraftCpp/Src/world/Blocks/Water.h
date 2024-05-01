#pragma once
#include "CubeBlock.h"

class Water : public CubeBlock
{
public:
	Water(int ID, int x, int y, int z, int texID) :CubeBlock(ID, x, y, z, texID, 1, true){}

	virtual void setOneFace(int face, bool state = true) { CubeBlock::setOneFace(face, state); }

	virtual void setOneFace(int face, Block* b) { setOneFace(face, false); }

	virtual glm::uvec2 getVertex(int dir);
};

