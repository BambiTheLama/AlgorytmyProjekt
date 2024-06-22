#pragma once
#include "CubeBlock.h"

class Leaf : public CubeBlock
{
public:
    Leaf(int ID, int x, int y, int z, int texID);

    bool getDisplay(Block* b) { return true; }

    virtual void setOneFace(int face, bool state = true) { CubeBlock::setOneFace(face, state); }

    virtual void setOneFace(int face, Block* b) { CubeBlock::setOneFace(face, true); }

    virtual glm::uvec2 getVertex(int dir);
};

