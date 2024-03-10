#pragma once
#include "Block.h"
#include <vector>
#define chunkW 16
#define chunkH 16
#define chunkT 16
class Game;
class Chunk
{
	
	int x, y, z;
	Block* blocks[chunkH][chunkW][chunkT];
	std::vector<Block*> toDelete;
	std::vector<Block*> toAdd;
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

	
};

