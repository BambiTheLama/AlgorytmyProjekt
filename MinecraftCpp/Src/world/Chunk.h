#pragma once
#include "Blocks/Block.h"
#include <vector>
#include <string>
#include "ChunkMesh.h"
#include <json.hpp>
#define chunkW 16
#define chunkH 256
#define chunkT 16
#define minH 5
#define maxH 256
#define waterH 75
//#define noSave

class Game;
struct SaveChunkData {
	int x, y, z;
	nlohmann::json j;
};
class Chunk
{
	int x, y, z;
	Block* blocks[chunkH][chunkW][chunkT];
	std::vector<Block*> toDelete;
	std::vector<Block*> toUpdate;
	std::vector<Block*> toAdd;
	std::vector<Block*> toDraw;
	ChunkMesh* mesh[10];
	int indicesSolidSize = 0;
	int indicesTransSize = 0;
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

	void deleteBlock(int x, int y, int z);

	bool addBlock(Block* b);

	bool isThisChunk(int x, int y, int z) const;

	void save();

	bool loadGame();

	bool loadGame(nlohmann::json j);

	glm::vec3 getPos() { return glm::vec3(x * chunkW + chunkW / 2, y * chunkH + chunkH / 2, z * chunkT + chunkT / 2); }

	glm::vec3 getLocation() { return glm::vec3(x, y, z); }

	void genVerticesFlag() { genVertices = true; }

	void clearBlocks();

	static void saveBlockToChunk(int x, int y, int z, int ID);

	static std::string fileName(int x, int y, int z) { return path + "chunk " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ".json"; }

	friend class Game;
private:

	static void saveBlockData();

	void setFacing();

	void genVerticesPos();

	void generateTerrain();

	void fillWater(int &x, int &z, int h, float &temperature);

	void genSandForWater(int &x, int &z, int y, int h);

	void genPlants(int &x, int &z, int y, float &temperature, float &structureNoise);

	void genStructures(int &x, int &z, int y, float &temperature, float &structureNoise);

	void genStone(int &x, int &z, int h);

	void biomLayer(int &x, int &z, int y, int h, float &temperature, float &structureNoise);

};

int getBlockX(int x);
int getBlockZ(int z);

