#pragma once
#include "Scene.h"
#include <vector> 
#include "../World/Chunk.h"
#include "../core/Camera.h"
#include <thread>
#include <mutex>
#include "GameTextures.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../world/SkyBox.h"

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
    int range = 5;
    VAO *vao;
    VBO *vbo;
    EBO *ebo;
    glm::vec3 chunkPos;
    std::vector<GLuint> index;
    std::vector<GLuint> vertices;
    Cube* cube;
    SkyBox* skybox;
    glm::vec3 cameraDir;
    glm::vec3 cameraPos;
    Shader* shader = NULL;
    Shader* shaderShadow = NULL;
    GameTextures* Blocks;
    GameTextures* BlocksH;
    GameTextures* BlocksN;
    Texture* waterTexture;
    Texture* waterTexture2;
    Texture* waterTextureN;

    RenderTexture* ShadowMap = NULL;
    RenderTexture* RefractionMap = NULL;
    static Game* game;
    float time = 0.0f;
    bool debug = false;
    bool reloadMesh = false;
    float lightDir[3] = { 0,1,0.0001 };
    float lightColor[3] = { 1,1,1 };
    bool stopGenDestyWorld = false;
    ImGuiIO* io;
public:
    Game(int w,int h, GLFWwindow* window, ImGuiIO* io);

    ~Game();

    void start();

    void update(float deltaTime);

    void draw();

    void renderScene(Shader* s, bool trans);

    void drawBlock();

    Block* getBlockAt(int x, int y, int z);

    glm::vec3 getChunkPos(int x, int y, int z);

    void deleteBlock(int x, int y, int z);

    bool isBlockAt(int x, int y, int z);

    bool addBlock(Block* b);

    void setFacing(int x,int y,int z,bool display, char face = 0b111111);

    void setFacing(Block* b, char face = 0b111111);

    Chunk* getChunkAt(int x, int y, int z);

    void setGenVerticesFlagAt(int x, int y, int z);

    void findLookingAtBLock();

    void reloadChunksNextTo(Chunk* c);

    void clearChunks();

    friend Game* getCurrentGame();

private:
    void worldGenerateFun();

    void worldDestroyFun();

    void genWorld();

    void desWorld();
};

Game* getCurrentGame();

void addObjectToSave(int x, int y, int z, int ID);