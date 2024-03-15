#pragma once
#include "Block.h"
#include <vector>
#define chunkW 16
#define chunkH 16
#define chunkT 16
#define minH 2
#define maxH 100
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
public:
	Chunk(int x, int y, int z);

	~Chunk();

	void update(float deltaTime);

	void draw();

	Block* getBlock(int x, int y, int z);

	void deleteBlock(Block* b);

	bool isThisChunk(int x, int y, int z);

	glm::vec3 getPos() { return glm::vec3(x * chunkW + chunkW / 2, y * chunkH + chunkH / 2, z * chunkT + chunkT / 2); }

	friend class Game;

private:
	void genVerticesPos();
	void genVerticesTexture();
	void genIndex();
	void generateTeren();
};

