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
    GLFWwindow* window;
public:
    Game(Camera *camera, GLFWwindow* window);

    ~Game();

    void update(float deltaTime);

    void draw();

    Block* getBlockAt(int x, int y, int z);

    void deleteBlock(Block* b);

    void sortChunks();

    void setFaceing(int x,int y,int z,bool display);

    void setFaceing(Block* b, bool display);
};

