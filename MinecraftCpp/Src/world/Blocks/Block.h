#pragma once
#include "../../core/Cube.h"
#include <vector>
#include <glm/glm.hpp>

class Texture;
class Block
{
	int ID;
protected:
	int x, y, z;
	char underWather = 0;
public:
	Block(int ID, int x, int y, int z);

	virtual ~Block(){}

	virtual void update(float deltaTime){}

	virtual void setFacing(int faces) = 0;

	virtual void setOneFace(int face, bool state = true) = 0;

	virtual void setOneFace(int face, Block* b) = 0;

	virtual bool isRenderedSide(int dir) { return false; }

	virtual glm::uvec2 getVertex(int dir) { return glm::uvec2(0, 0); }

	virtual char faceToSetUp() { return 0; }

	virtual bool isTransparent() { return true; }

	int getID()const { return ID; }

	virtual char getFaces()const { return 0; }

	virtual bool getDisplay(Block* b) { return b->isTransparent() != isTransparent(); }

	virtual bool isUpdateBlock() { return false; }

	void setFaceUnderWather(int face) { underWather = ((~face) & underWather) + face; }

	virtual bool isLiquid() { return false; }

	friend class Chunk;
	friend class Game;
};