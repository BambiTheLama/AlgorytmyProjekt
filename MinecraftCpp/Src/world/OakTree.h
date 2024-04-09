#pragma once
#include "Plant.h"
class OakTree :
    public Plant
{
    int woodID;
    int leafID;
public:
    OakTree(int ID, int x, int y, int z, int textureID,int woodID=7, int leafID=8);

    void update(float deltaTime);
};

