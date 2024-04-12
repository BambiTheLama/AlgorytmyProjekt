#pragma once
#include "CubeBlock.h"
class Leaf :
    public CubeBlock
{
public:
    Leaf(int ID, int x, int y, int z, int textID);

    bool getDisplay(Block* b) { return true; }

    virtual void setOneFace(int face, bool state) {}

    virtual void setOneFace(int face, Block* b) {  }
};

