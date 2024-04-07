#pragma once
#include "Plant.h"
class OakTree :
    public Plant
{
    int woodID;
    int leafID;
public:
    OakTree(int ID, int x, int y, int z, int textureX, int textureY,int woodID=7, int leafID=8);

    void update(float deltaTime);
};

