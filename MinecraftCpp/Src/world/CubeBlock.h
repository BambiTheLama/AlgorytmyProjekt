#pragma once
#include "Block.h"
#include "../core/Cube.h"
#include "../core/Texture.h"
class CubeBlock :
    public Block
{
	Cube* cube;
	int textureFaces;
	int textureID;
	bool transparent;
public:
	CubeBlock(int ID, int x, int y, int z, int textureID = 0 , int textureFaces = 1, bool transparent = false);

	~CubeBlock();

	virtual void setFaceing(int faces);

	virtual void setOneFace(int face, bool state = true);

	virtual std::vector<GLuint> getVertex();

	virtual std::vector<GLuint> getIndex();

	virtual char faceToSetUp() { return cube->getFaces(); }

	GLuint indexSize();

	virtual bool isTransparent() { return transparent; }

	virtual char getFaces()const { return cube->getFaces(); }
};

