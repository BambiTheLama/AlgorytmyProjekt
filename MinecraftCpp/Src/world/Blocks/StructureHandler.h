#pragma once
#include "Block.h"
class StructureHandler :
    public Block
{
    int* structure;
    int w, h,t;
    int timesToRotate = 0;
    int variant = 0;
public:
    StructureHandler(int ID, int x, int y, int z, int w, int h, int t);

    ~StructureHandler();

    void update(float deltaTime);

    void rotate();

    void setBlock(int x, int y, int z, int ID);

    void removeBlock(int x, int y,int z);

    void swapBlocks(int oldID, int newID);

    virtual void setFacing(int faces){}

    virtual void setOneFace(int face, bool state = true) {}

    virtual void setOneFace(int face, Block* b) {}

    virtual bool isUpdateBlock() { return true; }

    int getTimeRotated() { return timesToRotate; }

    int getVariant() { return variant; }

    void setVariant(int variant);
};

