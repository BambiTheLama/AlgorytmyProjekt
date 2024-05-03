#pragma once
#include "CubeBlock.h"

class Water : public CubeBlock
{
	bool liquid;
public:
	Water(int ID, int x, int y, int z, int texID,int textureSides=1,bool liquid=false) :CubeBlock(ID, x, y, z, texID, textureSides, true)
	{
		this->liquid = liquid;
	}

	virtual void setOneFace(int face, bool state = true) { CubeBlock::setOneFace(face, state); }

	virtual void setOneFace(int face, Block* b) { CubeBlock::setOneFace(face, false); }

	virtual glm::uvec2 getVertex(int dir);

	virtual bool isLiquid() { return liquid; }
};

