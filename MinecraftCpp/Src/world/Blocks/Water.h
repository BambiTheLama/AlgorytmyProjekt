#pragma once
#include "CubeBlock.h"

class Water : public CubeBlock
{
public:
	Water(int ID, int x, int y, int z, int texID,int textureSides=1) :CubeBlock(ID, x, y, z, texID, textureSides, true){}

	virtual void setOneFace(int face, bool state = true) { CubeBlock::setOneFace(face, state); }

	virtual void setOneFace(int face, Block* b) { setOneFace(face, false); }

	virtual glm::uvec2 getVertex(int dir);
};

