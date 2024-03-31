#pragma once
#include "Block.h"
#include "../core/Cube.h"
#include "../core/Texture.h"
class CubeBlock :
    public Block
{
	Cube* cube;
	int textureFaces;
	int textureX, textureY;
	bool transparent;
public:
	CubeBlock(int ID, int x, int y, int z, int textureX = 0, int textureY = 0, int textureFaces = 1, bool transparent = false);

	~CubeBlock();

	virtual void setFaceing(int faces);

	virtual void setOneFace(int face, bool state = true);

	virtual std::vector<GLuint> getVertex();

	virtual std::vector<GLuint> getIndex();

	GLuint indexSize();

	virtual bool isTransparent() { return transparent; }

	virtual char getFaces()const { return cube->getFaces(); }
};

