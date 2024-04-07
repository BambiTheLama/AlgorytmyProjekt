#include "CubeBlock.h"
#include "../core/Engine.h"
#include "Chunk.h"

CubeBlock::CubeBlock(int ID, int x, int y, int z, int textureX, int textureY, int textureFaces, bool transparent) :Block(ID, x, y, z)
{
	cube = new Cube();
	this->textureFaces = textureFaces;
	this->textureX = textureX;
	this->textureY = textureY;
	this->transparent = transparent;
}

CubeBlock::~CubeBlock()
{
	delete cube;
}

void CubeBlock::setFaceing(int faces)
{
	//texture->useTexture()
	cube->setFaceing(faces);
}

void CubeBlock::setOneFace(int face, bool state)
{
	cube->setOneFace(face, state);
}

std::vector<GLuint> CubeBlock::getVertex()
{
	int x = this->x;
	int z = this->z;
	if (x >= 0)
		x %= chunkW;
	else
		x = (chunkW  - (abs(x) % chunkW)) % chunkW;
	if (z >= 0)
		z %= chunkT;
	else
		z = (chunkT  - (abs(z) % chunkT)) % chunkT;
	return cube->getVertex(x, y, z, textureFaces, textureX, textureY);
}

std::vector<GLuint> CubeBlock::getIndex()
{
	return cube->getIndex(transparent);
}
GLuint CubeBlock::indexSize()
{
	return cube->indexSize();
}