#pragma once
#include "Block.h"
#include "Cube.h"
#include "Texture.h"
class CubeBlock :
    public Block
{
	Cube* cube;
	Texture* texture;
public:
	CubeBlock(int x, int y, int z,const char* path);

	~CubeBlock();

	virtual void draw();

	virtual void setFaceing(Faces faces);

	virtual void setOneFace(Faces face, bool state);
};

