#pragma once
#include "Block.h"
class StructureHalder :
    public Block
{
    int* structure;
    int w, h,t;
    int timesToRotate = 0;
public:
    StructureHalder(int ID, int x, int y, int z, int w, int h, int t);

    ~StructureHalder();

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
};

