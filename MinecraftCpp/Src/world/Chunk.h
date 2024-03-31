#pragma once
#include "Block.h"
#include <vector>
#include <string>
#include "PerlinNoise.h"
#define chunkW 15
#define chunkH 255
#define chunkT 15
#define minH 8
#define maxH 255
#define waterH 69
//#define Laby
#define noSave

class Game;
class VBO;
class VAO;
class EBO;
class Chunk
{
#ifdef Laby
	static PerlinNoice noise;
	static PerlinNoice noise2;
	static PerlinNoice noiseRiver;
#endif // Laby
	int x, y, z;
	Block* blocks[chunkH][chunkW][chunkT];
	std::vector<Block*> toDelete;
	std::vector<Block*> toAdd;
	std::vector<Block*> toDraw;
	VAO* vaoS = NULL;
	VBO* vboS = NULL;
	EBO* eboS = NULL;
	VAO* vaoT = NULL;
	VBO* vboT = NULL;
	EBO* eboT = NULL;
	std::vector<GLuint> verticesSolid;
	std::vector<GLuint> indicesSolid;
	std::vector<GLuint> verticesTrans;
	std::vector<GLuint> indicesTrans;
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

	bool isThisChunk(int x, int y, int z);

	void save();

	bool loadGame();

	glm::vec3 getPos() { return glm::vec3(x * chunkW + chunkW / 2, y * chunkH + chunkH / 2, z * chunkT + chunkT / 2); }

	glm::vec3 getLocation() { return glm::vec3(x, y, z); }

	void genVerticesFlag() { genVertices = true; }

	void clearBlocks();

	friend class Game;

private:
#ifdef Laby
	float getNoiseValue(int x, int z);
#endif
	void genVerticesPos();
	void generateTeren();
	void setFaceing();
	void genBiom(int x, int z, int startY, int endY,float temperature);
};
void setNoiseSeed(int seed);

