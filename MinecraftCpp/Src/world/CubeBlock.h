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
public:
	CubeBlock(int x, int y, int z,int textureX=0,int textureY=0, int textureFaces=1);

	~CubeBlock();

	virtual void setFaceing(int faces);

	virtual void setOneFace(int face, bool state = true);

	std::vector<glm::vec3> getVertexPos();

	std::vector<glm::vec2> getVertexTexture();

	std::vector<GLuint> getIndex();

	GLuint indexSize();
};

