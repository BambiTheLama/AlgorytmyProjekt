#pragma once
#include "CubeBlock.h"
class Cactus :
    public CubeBlock
{
public:
    Cactus(int ID, int x, int y, int z, int textureID);

    GLuint getVertex(int dir);
};

