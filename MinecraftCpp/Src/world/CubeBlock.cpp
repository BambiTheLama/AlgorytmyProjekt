#include "CubeBlock.h"
#include "../core/Engine.h"

CubeBlock::CubeBlock(int x, int y, int z, const char* path):Block(x,y,z)
{
	cube = new Cube();

	texture = new Texture(path);
}

CubeBlock::~CubeBlock()
{
	delete cube;
	delete texture;
}

void CubeBlock::draw()
{
	texture->useTexture("tex0", 0);
	texture->bind();
	cube->draw(x, y, z);
}
void CubeBlock::drawSelect()
{
	Texture* t = getSelect();
	t->useTexture("tex0", 0);
	t->bind();
	cube->draw(x, y, z);
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