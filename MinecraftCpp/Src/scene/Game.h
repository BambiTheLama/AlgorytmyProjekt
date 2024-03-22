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
    std::thread worldDestroyT;
    std::mutex toAddMutex;
    std::mutex toDeleteMutex;
    std::mutex chunksMutex;
    std::vector<Chunk*> chunks;
    std::vector<Chunk*> toAdd;
    std::vector<Chunk*> toDelete;
    std::vector<Chunk*> toSave;
    Camera *camera;
    GLFWwindow* window;
    Block* b = NULL;
    std::vector<glm::vec2> posToGenChunk;
    int range = 6;
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

    void draw(Shader* s);

    Block* getBlockAt(int x, int y, int z);

    void deleteBlock(Block* b);

    void setFaceing(int x,int y,int z,bool display, char face = 0b111111);

    void setFaceing(Block* b,int x,int y,int z, bool display, char face = 0b111111);

    void setGenVerticesFlagAt(int x, int y, int z);

private:
    void worldGenerateFun();

    void worldDestroyFun();

    void genWorld();

    void desWorld();
};

