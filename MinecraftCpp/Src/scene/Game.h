#pragma once
#include "Scene.h"
#include <vector> 
#include "../World/Chunk.h"
#include "../core/Camera.h"

class Game :
    public Scene
{
    std::vector<Chunk*> chunks;
    std::vector<Chunk*> toAdd;
    std::vector<Chunk*> toDelete;
    Camera *camera;
public:
    Game(Camera *camera);

    ~Game();

    void update(float deltaTime);

    void draw();

    Block* getBlockAt(int x, int y, int z);

    void sortChunks();
};

