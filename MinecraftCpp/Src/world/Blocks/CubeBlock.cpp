#include "CubeBlock.h"
#include "../../core/Engine.h"
#include "../Chunk.h"

CubeBlock::CubeBlock(int ID, int x, int y, int z, int textureID, int textureFaces, bool transparent) :Block(ID, x, y, z)
{
	cube = new Cube();
	this->textureFaces = textureFaces;
	this->textureID = textureID;
	this->transparent = transparent;
}

CubeBlock::~CubeBlock()
{
	delete cube;
}

void CubeBlock::setFacing(int faces)
{
	cube->setFacing(faces);
}

void CubeBlock::setOneFace(int face, bool state)
{
	cube->setOneFace(face, state);
	if (!state && underWater > 0)
	{
		underWater = (~face) & underWater + face * state;
	}
}
void CubeBlock::setOneFace(int face, Block* b) 
{
	if (transparent)
		setOneFace(face, b->getID() != getID());
	else
		setOneFace(face, getDisplay(b));
	if(b->isLiquid())
		setFaceUnderWater(face);

}

glm::uvec2 CubeBlock::getVertex(int dir)
{
	int x = this->x;
	int z = this->z;
	int d = 0b1 << (dir);
	return glm::uvec2(cube->getVertex(x, y, z, textureFaces, textureID, d), (underWater & d) > 0 ? 16 : 0);
}
