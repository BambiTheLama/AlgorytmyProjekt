#include "Water.h"

glm::uvec2 Water::getVertex(int dir)
{
	return CubeBlock::getVertex(dir) + glm::uvec2(0, 1 + (liquid ? 32: 0));
}