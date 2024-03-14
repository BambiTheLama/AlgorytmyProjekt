#include "CubeBlock.h"
#include "../core/Engine.h"

CubeBlock::CubeBlock(int x, int y, int z, int textureX, int textureY, int textureFaces) :Block(x,y,z)
{
	cube = new Cube();
	this->textureFaces = textureFaces;
	this->textureX = textureX;
	this->textureY = textureY;
}

CubeBlock::~CubeBlock()
{
	delete cube;
}

void CubeBlock::setFaceing(int faces)
{
	//texture->useTexture()
	cube->setFaceing(faces);
}

void CubeBlock::setOneFace(int face, bool state)
{
	cube->setOneFace(face, state);
}

std::vector<glm::vec3> CubeBlock::getVertexPos()
{
	std::vector<glm::vec3> vert = cube->getVertexPos();
	std::vector<glm::vec3> vert2;
	for (auto v : vert)
	{
		vert2.push_back(glm::vec3(v.x + x, v.y + y, v.z + z));
	}
	return vert2;
}

std::vector<glm::vec2> CubeBlock::getVertexTexture()
{
	std::vector<glm::vec2> text = cube->getVertexTexture(textureFaces);
	for (auto t : text)
	{
		t.x = (textureX + t.x);
		t.y = (textureY + t.y);
	}
	return text;
}

std::vector<GLuint> CubeBlock::getIndex()
{
	return cube->getIndex();
}
GLuint CubeBlock::indexSize()
{
	return cube->indexSize();
}