#pragma once
#include "Block.h"
class Plant :
    public Block
{
	int textureX;
	int textureY;
public:
	Plant(int ID, int x, int y, int z, int textureX, int textureY);

	virtual ~Plant() {}

	virtual void setFaceing(int faces) {}

	virtual void setOneFace(int face, bool state = true) {}

	virtual std::vector<GLuint> getVertex();

	virtual std::vector<GLuint> getIndex();

	virtual GLuint indexSize();

	virtual char faceToSetUp() { return 0; }

	virtual bool isTransparent() { return true; }

	virtual char getFaces()const { return 0b111111; }
};

