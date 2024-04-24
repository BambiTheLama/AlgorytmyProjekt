#include "Leaf.h"

Leaf::Leaf(int ID, int x, int y, int z, int texID) :CubeBlock(ID, x, y, z,texID, 1, true)
{

}

GLuint Leaf::getVertex(int dir)
{
	return CubeBlock::getVertex(dir) + (0b11 << 24);
}