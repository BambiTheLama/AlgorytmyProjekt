#pragma once
#include "Block.h"
class Plant : public Block
{
	int textureID;
public:
	Plant(int ID, int x, int y, int z, int textureID);

	virtual ~Plant() {}

	virtual void update(float deltaTime);

	virtual void setFacing(int faces) {}

	virtual void setOneFace(int face, bool state = true) {}

	virtual void setOneFace(int face, Block* b) {}

	virtual glm::uvec2 getVertex(int dir);

	virtual bool isRenderedSide(int dir) {
		return dir > 5;
	}

	virtual char faceToSetUp() { return 0; }

	virtual bool isTransparent() { return true; }

	virtual char getFaces()const { return 0b111111; }

private:
	std::vector<glm::vec3> getPos(int x,int y,int z);
};