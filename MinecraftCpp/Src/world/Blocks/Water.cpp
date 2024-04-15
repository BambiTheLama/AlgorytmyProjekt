#include "Water.h"

GLuint Water::getVertex(int dir)
{

	return CubeBlock::getVertex(dir) + (1 << 23);

}