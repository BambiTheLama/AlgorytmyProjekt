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


    virtual void setFacing(int faces) {}

    virtual void setOneFace(int face, bool state = true) {}

    virtual void setOneFace(int face, Block* b){}

    virtual std::vector<GLuint> getVertex() { return std::vector<GLuint>(); }

    virtual std::vector<GLuint> getIndex() { return std::vector<GLuint>(); }

    virtual GLuint indexSize() { return 0; }

    virtual bool isUpdateBlock() { return true; }

};

