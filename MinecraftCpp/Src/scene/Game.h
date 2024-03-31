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
    std::vector<Chunk*> toDraw;
    Camera *camera;
    GLFWwindow* window;
    Block* b = NULL;
    std::vector<glm::vec2> posToGenChunk;
    int range = 10;
    VAO *vao;
    VBO *vbo;
    EBO *ebo;
    Texture* selection;
    glm::vec3 chunkPos;
    std::vector<GLuint> index;
    std::vector<GLuint> vertices;
    Cube* cube;
public:
    Game(Camera *camera, GLFWwindow* window);

    ~Game();

    void start();

    void update(float deltaTime);

    void draw(Shader* s);

    Block* getBlockAt(int x, int y, int z);

    glm::vec3 getChunkPos(int x, int y, int z);

    void deleteBlock(int x, int y, int z);

    void setFaceing(int x,int y,int z,bool display, char face = 0b111111);

    void setFaceing(Block* b,int x,int y,int z, char face = 0b111111);

    void setGenVerticesFlagAt(int x, int y, int z);

private:
    void worldGenerateFun();

    void worldDestroyFun();

    void genWorld();

    void desWorld();
};

