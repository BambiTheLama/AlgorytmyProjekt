#pragma once
#include "Block.h"
#include <vector>
#include <string>
#define chunkW 16
#define chunkH 128
#define chunkT 16
#define minH 3
#define maxH 128
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
	VBO *vboVert = NULL;
	VBO *vboTexture = NULL;
	EBO *index = NULL;
	std::vector<glm::vec3> vertV;
	std::vector<glm::vec2> textV;
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

	void draw();

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
	void genVerticesTexture();
	void genIndex();
	void generateTeren();
	void setFaceing();
};

