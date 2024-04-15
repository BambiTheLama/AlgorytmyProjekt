#include "Leaf.h"

Leaf::Leaf(int ID, int x, int y, int z, int textID) :CubeBlock(ID, x, y, z,textID, 1, true)
{

}

GLuint Leaf::getVertex(int dir)
{
	return CubeBlock::getVertex(dir) + (0b11 << 25);
}