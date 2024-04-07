#pragma once
#include "CubeBlock.h"
class Leaf :
    public CubeBlock
{
public:
    Leaf(int ID, int x, int y, int z, int textureX, int textureY);

    bool getDisplay(Block* b) { return true; }

};

