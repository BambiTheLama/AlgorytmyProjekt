#pragma once
#include "Scene.h"
#include <vector> 
#include "../World/Chunk.h"
#include "../core/Camera.h"
#include <thread>
#include <mutex>
#include "GameTextures.h"

class Texture;
class RenderTexture;
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
    glm::vec3 blockPos;
    std::vector<glm::vec2> posToGenChunk;
    int range = 16;
    VAO *vao;
    VBO *vbo;
    EBO *ebo;
    Texture* selection;
    glm::vec3 chunkPos;
    std::vector<GLuint> index;
    std::vector<GLuint> vertices;
    Cube* cube;

    glm::vec3 cameraDir;
    glm::vec3 cameraPos;
    Shader* shader = NULL;
    Shader* shaderShadow = NULL;
    GameTextures* Blocks;
    GameTextures* BlocksH;
    GameTextures* BlocksN;

    RenderTexture* ShadowMap = NULL;
    static Game* game;
    float time = 0.0f;
    bool debug = false;
    bool reloadMesh = false;

public:
    Game(int w,int h, GLFWwindow* window);

    ~Game();

    void start();

    void update(float deltaTime);

    void draw();

    void renderScene(Shader* s, bool trans);

    void drawBlock();

    Block* getBlockAt(int x, int y, int z);

    glm::vec3 getChunkPos(int x, int y, int z);

    void deleteBlock(int x, int y, int z);

    bool addBlock(Block* b);

    void setFacing(int x,int y,int z,bool display, char face = 0b111111);

    void setFacing(Block* b, char face = 0b111111);

    Chunk* getChunkAt(int x, int y, int z);

    void setGenVerticesFlagAt(int x, int y, int z);

    void findLookingAtBLock();

    void reloadChunksNextTo(Chunk* c);

    friend Game* getCurrentGame();

private:
    void worldGenerateFun();

    void worldDestroyFun();

    void genWorld();

    void desWorld();
};

Game* getCurrentGame();

void addObjectToSave(int x, int y, int z, int ID);