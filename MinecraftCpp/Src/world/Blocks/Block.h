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
public:
	Block(int ID, int x, int y, int z);

	virtual ~Block(){}

	virtual void update(float deltaTime){}

	virtual void setFaceing(int faces) = 0;

	virtual void setOneFace(int face, bool state = true) = 0;

	virtual void setOneFace(int face, Block* b) = 0;

	virtual std::vector<GLuint> getVertex() = 0;

	virtual bool isRenderedSide(int dir) { return false; }

	virtual GLuint getVertex(int dir) { return 0; }

	virtual std::vector<GLuint> getIndex() = 0;

	virtual GLuint indexSize() = 0;

	virtual char faceToSetUp() { return 0; }

	virtual bool isTransparent() { return true; }

	int getID()const { return ID; }

	virtual char getFaces()const { return 0; }

	virtual bool getDisplay(Block* b) { return b->isTransparent() != isTransparent(); }

	virtual bool isUpdateBlock() { return false; }

	friend class Chunk;
	friend class Game;
};

