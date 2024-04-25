#pragma once
#include "Block.h"
class CubeHouse : public Block
{
    int woodID, plankID, floorID;
public:
    CubeHouse(int ID, int x, int y, int z, int woodID = 0, int plankID = 0, int floorID = 0);

    virtual ~CubeHouse() {}

    virtual void update(float deltaTime);

    virtual void setFacing(int faces) {}

    virtual void setOneFace(int face, bool state = true) {}

    virtual void setOneFace(int face, Block* b){}

    virtual bool isUpdateBlock() { return true; }
};