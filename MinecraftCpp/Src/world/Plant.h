#pragma once
#include "Block.h"
class Plant :
    public Block
{
	int textureID;
public:
	Plant(int ID, int x, int y, int z, int textureID);

	virtual ~Plant() {}

	virtual void update(float deltaTime);

	virtual void setFaceing(int faces) {}

	virtual void setOneFace(int face, bool state = true) {}

	virtual void setOneFace(int face, Block* b) {}

	virtual std::vector<GLuint> getVertex();

	virtual std::vector<GLuint> getIndex();

	virtual GLuint indexSize();

	virtual char faceToSetUp() { return 0; }

	virtual bool isTransparent() { return true; }

	virtual char getFaces()const { return 0b111111; }

private:
	std::vector<glm::vec3> getPos(int x,int y,int z);


};

