#pragma once
#include "Block.h"
class CubeHouse :
    public Block
{
    int woodID, plankID, florID;
public:
    CubeHouse(int ID, int x, int y, int z, int woodID = 0, int plankID = 0, int florID = 0);

    virtual ~CubeHouse() {}

    virtual void update(float deltaTime);

    virtual void setFaceing(int faces);
};

