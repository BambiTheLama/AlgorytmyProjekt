#include "Leaf.h"

Leaf::Leaf(int ID, int x, int y, int z, int texID) :CubeBlock(ID, x, y, z,texID, 1, true)
{
}

glm::uvec2 Leaf::getVertex(int dir)
{
	return glm::uvec2(CubeBlock::getVertex(dir).x, 12);
}