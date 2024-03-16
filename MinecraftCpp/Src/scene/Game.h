#pragma once
#include "Scene.h"
#include <vector> 
#include "../World/Chunk.h"
#include "../core/Camera.h"
#include <thread>
#include <mutex>


class Game :
    public Scene
{
    bool gameRunning;
    std::thread worldGenerateT;
    std::mutex toAddMutex;
    std::mutex chunksMutex;
    std::vector<Chunk*> chunks;
    std::vector<Chunk*> toAdd;
    std::vector<Chunk*> toDelete;
    Camera *camera;
    GLFWwindow* window;
    Block* b = NULL;
    std::vector<glm::vec3> posToGenChunk;
    int range = 4;
    VAO *vao;
    VBO *vboPos;
    VBO *vboTex;
    EBO *ebo;
    Texture* selection;
public:
    Game(Camera *camera, GLFWwindow* window);

    ~Game();

    void start();

    void update(float deltaTime);

    void draw();

    void endPhase();

    Block* getBlockAt(int x, int y, int z);

    void deleteBlock(Block* b);

    void setFaceing(int x,int y,int z,bool display, char face = 0b111111);

    void setFaceing(Block* b, bool display, char face = 0b111111);

    void setGenVerticesFlagAt(int x, int y, int z);

private:
    void worldGenerateFun();

    void genWorld();
};

