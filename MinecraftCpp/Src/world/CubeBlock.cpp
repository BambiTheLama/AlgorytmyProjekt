#include "CubeBlock.h"

CubeBlock::CubeBlock(int x, int y, int z, const char* path):Block(x,y,z)
{
	cube = new Cube();
	texture = new Texture(path);
}

CubeBlock::~CubeBlock()
{

}

void CubeBlock::draw()
{
	cube->draw(x, y, z);
}

void CubeBlock::setFaceing(Faces faces)
{
	//texture->useTexture()
	//cube->setFaceing(faces);
}

void CubeBlock::setOneFace(Faces face, bool state)
{
	//cube->setOneFace(face, state);
}