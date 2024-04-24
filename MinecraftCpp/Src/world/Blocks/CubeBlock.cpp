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
	//texture->useTexture()
	cube->setFacing(faces);
}

void CubeBlock::setOneFace(int face, bool state)
{
	cube->setOneFace(face, state);
}
void CubeBlock::setOneFace(int face, Block* b) 
{
	if (transparent)
		setOneFace(face, b->getID() != getID());
	else
		setOneFace(face, getDisplay(b));
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
	return cube->getVertex(x, y, z, textureFaces, textureID);
}

GLuint CubeBlock::getVertex(int dir)
{
	int x = this->x;
	int z = this->z;
	if (x >= 0)
		x %= chunkW;
	else
		x = (chunkW - (abs(x) % chunkW)) % chunkW;
	if (z >= 0)
		z %= chunkT;
	else
		z = (chunkT - (abs(z) % chunkT)) % chunkT;
	return cube->getVertex(x, y, z, textureFaces, textureID, 0b1 << (dir));
}

std::vector<GLuint> CubeBlock::getIndex()
{
	return cube->getIndex(transparent);
}
GLuint CubeBlock::indexSize()
{
	return cube->indexSize();
}