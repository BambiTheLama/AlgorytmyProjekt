#pragma once
#include "Blocks/Block.h"
#include <vector>
#include <string>
#include "ChunkMesh.h"
#include <json.hpp>
#define chunkW 32
#define chunkH 256
#define chunkT 32
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
	static Game* game;
	static int seed;
	bool genVertices = false;
	bool wasCleared = false;
	static std::string path;
	static std::vector<SaveChunkData*> data;
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

	bool isBlockAt(int x, int y, int z);

	void save();

	bool loadGame();

	bool loadGame(nlohmann::json j);

	glm::vec3 getPos() { return glm::vec3(x * chunkW + chunkW / 2, y * chunkH + chunkH / 2, z * chunkT + chunkT / 2); }

	glm::vec3 getLocation() { return glm::vec3(x, y, z); }

	void genVerticesFlag() { genVertices = true; }

	void clearBlocks();

	static void saveBlockToChunk(int x, int y, int z, int ID);

	static std::string fileName(int x, int y, int z) { return path + "chunk " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ".json"; }

	void reloadBlocksFront();

	void reloadBlocksBack();

	void reloadBlocksLeft();

	void reloadBlocksRight();

	friend class Game;
private:
	static void saveBlockToJson(nlohmann::json &j,int &x, int &y, int &z, int &ID);

	static void readBlockToJson(nlohmann::json& j, int &x, int &y, int &z, int &ID);

	static void saveBlockData();

	static bool hasDataToRead(int x, int y, int z);

	static SaveChunkData* getChunkData(int x, int y, int z);

	void setFacing();

	void readDataFromDataFile(SaveChunkData* saveData);

	void genVerticPos(int dir);

	void genVerticesPos();

	void generateTerrain();

	void fillWater(int &x, int &z, int h, float &temperature);

	void genSandForWater(int &x, int &z, int y, int h);

	void genPlants(int &x, int &z, int y, float &temperature, float &structureNoise);

	void genStructures(int &x, int &z, int y, float &temperature, float &structureNoise);

	void genStone(int &x, int &z, int h);

	void biomLayer(int &x, int &z, int y, int h, float &temperature, float &structureNoise);

	void setBlockAt(Block* b,int x, int y, int z);

};

int getBlockX(int x);
int getBlockZ(int z);

