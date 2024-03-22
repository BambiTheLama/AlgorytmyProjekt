#pragma once
#include "Block.h"
#include "../core/Cube.h"
#include "../core/Texture.h"
class CubeBlock :
    public Block
{
	char toSetUp = 0b111111;
	Cube* cube;
	int textureFaces;
	int textureX, textureY;
public:
	CubeBlock(int ID, int x, int y, int z, int textureX = 0, int textureY = 0, int textureFaces = 1);

	~CubeBlock();

	virtual void setFaceing(int faces);

	virtual void setOneFace(int face, bool state = true);

	std::vector<GLuint> getVertex();

	std::vector<GLuint> getIndex();

	GLuint indexSize();

	virtual char faceToSetUp() { return toSetUp; }

	virtual bool isTransparent() { return false; }
};

