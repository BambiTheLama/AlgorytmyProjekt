#include "Cactus.h"

Cactus::Cactus(int ID, int x, int y, int z, int textureID) :CubeBlock(ID, x, y, z, textureID, 3, true)
{
}

glm::uvec2 Cactus::getVertex(int dir)
{
    if (checkFace(Up, 1 << dir) || checkFace(Down, 1 << dir))
    {
        return CubeBlock::getVertex(dir);
    }

    return glm::uvec2(CubeBlock::getVertex(dir).x, 2);
}