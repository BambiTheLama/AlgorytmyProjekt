#pragma once
#include "Block.h"
#include "../core/Cube.h"
#include "../core/Texture.h"
class CubeBlock :
    public Block
{
	Cube* cube;
	Texture* texture;
public:
	CubeBlock(int x, int y, int z, const char* path);

	~CubeBlock();

	virtual void draw();

	virtual void drawSelect();

	virtual void setFaceing(int faces);

	virtual void setOneFace(int face, bool state = true);
};

