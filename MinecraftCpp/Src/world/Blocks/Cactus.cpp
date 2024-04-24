#include "Cactus.h"

Cactus::Cactus(int ID, int x, int y, int z, int textureID) :CubeBlock(ID, x, y, z, textureID, 3, true)
{
}

GLuint Cactus::getVertex(int dir)
{
    if (checkFace(Up, 1 << dir) || checkFace(Down, 1 << dir))
    {
        return CubeBlock::getVertex(dir);
    }

    return CubeBlock::getVertex(dir) + (0b1 << 23);
}