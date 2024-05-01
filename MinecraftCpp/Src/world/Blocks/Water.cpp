#include "Water.h"

glm::uvec2 Water::getVertex(int dir)
{
	return glm::uvec2(CubeBlock::getVertex(dir).x, 1);
}