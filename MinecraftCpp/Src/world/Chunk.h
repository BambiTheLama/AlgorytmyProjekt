#pragma once
#include "Block.h"
#include <vector>
#include <string>
#define chunkW 1
#define chunkH 1
#define chunkT 1
#define minH 3
#define maxH 32
#define waterH 4
class Game;
class VBO;
class VAO;
class EBO;
class Chunk
{
	int x, y, z;
	Block* blocks[chunkH][chunkW][chunkT];
	std::vector<Block*> toDelete;
	std::vector<Block*> toAdd;
	std::vector<Block*> toDraw;
	VAO* vao = NULL;
	VBO *vbo = NULL;
	EBO *ebo = NULL;
	std::vector<int> vertices;
	std::vector<glm::vec3> verticesV;
	std::vector<glm::vec2> verticesT;
	std::vector<GLuint> indexV;
	static Game* game;
	bool genVertices = false;
	bool wasCleared = false;
	static std::string path;
public:
	Chunk(int x, int y, int z);

	~Chunk();

	void start();

	void update(float deltaTime);

	void draw(Shader* s);

	Block* getBlock(int x, int y, int z);

	void deleteBlock(Block* b);

	bool isThisChunk(int x, int y, int z);

	void save();

	bool loadGame();

	glm::vec3 getPos() { return glm::vec3(x * chunkW + chunkW / 2, y * chunkH + chunkH / 2, z * chunkT + chunkT / 2); }

	glm::vec3 getLocation() { return glm::vec3(x, y, z); }

	void genVerticesFlag() { genVertices = true; }

	void clearBlocks();

	friend class Game;

private:
	void genVerticesPos();
	void generateTeren();
	void setFaceing();
};

