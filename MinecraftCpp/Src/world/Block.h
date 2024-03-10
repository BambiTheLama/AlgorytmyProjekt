#pragma once
#include "Cube.h"

class Block
{
protected:
	int x, y, z;
public:
	Block(int x,int y,int z);

	virtual ~Block(){}

	virtual void draw() = 0;

	virtual void setFaceing(Faces faces) = 0;

	virtual void setOneFace(Faces face, bool state) = 0;
};

