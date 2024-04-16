#pragma once
#include "Plant.h"

class SpruceTree:
    public Plant
{
    int woodID;
    int leafID;
public:
    SpruceTree(int ID, int x, int y, int z, int textureID, int woodID = 7, int leafID = 8);

    void update(float deltaTime);

    virtual bool isUpdateBlock() { return true; }
};

