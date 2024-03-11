#pragma once
#include "../core/Cube.h"
class Texture;
class Block
{
	
protected:
	int x, y, z;
public:
	Block(int x,int y,int z);

	virtual ~Block(){}

	virtual void draw() = 0;

	virtual void drawSelect() = 0;

	virtual void setFaceing(int faces) = 0;

	virtual void setOneFace(int face, bool state = true) = 0;

	friend class Chunk;
	friend class Game;
};

