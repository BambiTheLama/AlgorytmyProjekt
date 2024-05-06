#include "Chunk.h"
#include "Blocks/CubeBlock.h"
#include "../scene/Game.h"
#include "FastNoiseLite.h"
#include "../core/VAO.h"
#include "../core/VBO.h"
#include "../core/EBO.h"
#include "Blocks/BlocksCreator.h"
#include <json.hpp>
#include <fstream>
#include "../Properties.h"
#include <direct.h>
#include "../core/Engine.h"
#include "../core/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include "Blocks/WaveColapseFunction.h"


#define forAllBlocks 	for (int j = 0; j < chunkH; j++)\
							for (int i = 0; i < chunkW; i++)\
								for (int k = 0; k < chunkT; k++)
Game* Chunk::game = NULL;
std::string Chunk::path = "World/";
int Chunk::seed = 69;
std::vector<SaveChunkData*> Chunk::data;

Chunk::Chunk(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;

	for (int i = 0; i < 10; i++)
	{
		mesh[i] = new ChunkMesh(i);
	}

	forAllBlocks
	{
		blocks[j][i][k] = NULL;
	}
	SaveChunkData* saveData = getChunkData(x, y, z);
	if (saveData)
	{
		if (!loadGame(saveData->j))
			generateTerrain();
		auto iterator = std::find(data.begin(), data.end(), saveData);
		data.erase(iterator);
		readDataFromDataFile(saveData);
		setFacing();
		delete saveData;
		return;
	}
	else
	{
		if (!loadGame())
			generateTerrain();
	}
	

	if (toAdd.size() <= 0 && toDelete.size() <= 0)
		setFacing();
}

Chunk::~Chunk()
{
	for (int i = 0; i < 10; i++)
		delete mesh[i];
	if (wasCleared)
		return;
	forAllBlocks
	if (blocks[j][i][k])
	{
		delete blocks[j][i][k];
	}
}

void Chunk::start()
{
	for (int i = 0; i < 10; i++)
	{
		mesh[i]->start();
		mesh[i]->genMesh();
	}	
	SaveChunkData* data = getChunkData(x, y, z);
	if (data)
	{
		auto it = std::find(this->data.begin(), this->data.end(), data);
		this->data.erase(it);
		readDataFromDataFile(data);
		delete data;
	}
}

void Chunk::update(float deltaTime)
{

	for (auto u : toUpdate)
	{
		u->update(deltaTime);
	}
	for (auto b : toDelete)
	{
		if (!b)
			continue;

		int by = b->y;
		int bz = getBlockZ(b->z);
		int bx = getBlockX(b->x);

		blocks[by][bx][bz] = NULL;
		game->setFacing(b->x, b->y, b->z, true);

		auto it = std::find(toUpdate.begin(), toUpdate.end(), b);
		if (it != toUpdate.end())
			toUpdate.erase(it);
		it = std::find(toAdd.begin(), toAdd.end(), b);
		if (it != toAdd.end())
			toAdd.erase(it);

		delete b;
	}

	for (auto b : toAdd)
	{
		if (b->faceToSetUp() > 0)
			game->setFacing(b, b->faceToSetUp());
		int by = b->y;
		int bz = getBlockZ(b->z);
		int bx = getBlockX(b->x);
		if (!blocks[by][bx][bz])
			blocks[by][bx][bz] = b;
		if (b->isUpdateBlock() && std::count(toUpdate.begin(), toUpdate.end(), b) <= 0)
		{
			toUpdate.push_back(b);
		}


	}
	if (genVertices || toAdd.size() > 0 || toDelete.size() > 0)
	{
		genVertices = false;
		toDelete.clear();
		toAdd.clear();
		genVerticesPos();
		for (int i = 0; i < 10; i++)
			mesh[i]->genMesh();
	}
}

void Chunk::draw(Shader* s)
{
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(x * chunkW, y * chunkH, z * chunkT));
	s->setUniformI1(x * chunkW, "chunkX");
	s->setUniformI1(z * chunkT, "chunkZ");
	for (int i = 0; i < 10; i++)
	{
		mesh[i]->draw(s);
	}
}

Block* Chunk::getBlock(int x, int y, int z)
{
	int bx = getBlockX(x);
	int by = y;
	int bz = getBlockZ(z);
	if (bx >= 0 && bx < chunkW && by >= 0 && by < chunkH && bz >= 0 && bz < chunkT)
	{
		if(blocks[by][bx][bz])
			return blocks[by][bx][bz];
		for (auto a : toAdd)
		{
			if (x == a->x && y == a->y && z == a->z)
				return a;
		}
	}
	return NULL;
}

void Chunk::deleteBlock(int x, int y, int z)
{
	int bx = getBlockX(x);
	int by = y;
	int bz = getBlockZ(z);
	if (bx >= 0 && bx < chunkW && by >= 0 && by < chunkH && bz >= 0 && bz < chunkT)
	{

		int removeID = -1;
		Block* b = NULL;
		for (int i = 0; i < toAdd.size(); i++)
			if (toAdd[i]->x == x && toAdd[i]->y == y && toAdd[i]->z == z)
			{
				removeID = i;
				b = toAdd[i];
				break;
			}
		if (removeID >= 0)
		{
			toAdd.erase(toAdd.begin() + removeID);
			removeID = -1;
			auto it = std::find(toUpdate.begin(), toUpdate.end(), b);
			if (it != toUpdate.end())
				toUpdate.erase(it);

			delete b;
		}

		for (auto d : toDelete)
			if (d == blocks[by][bx][bz])
				return;
		toDelete.push_back(blocks[by][bx][bz]);
	}
	game->setFacing(x, y, z, true);
}

bool Chunk::addBlock(Block* b)
{
	int x = getBlockX(b->x);
	int y = b->y;
	int z = getBlockZ(b->z);

	if (!blocks[y][x][z])
	{
		for (auto a : toAdd)
			if (b->z == a->z && b->x == a->x && b->y == a->y)
				return false;
		toAdd.push_back(b);
		return true;
	}
	for (auto d : toDelete)
		if (d)
		{
			if (b->z == d->z && b->x == d->x && b->y == d->y)
			{
				for (auto a : toAdd)
					if (b->z == a->z && b->x == a->x && b->y == a->y)
						return false;
				toAdd.push_back(b);
				return true;
			}
		}

	return false;
}

bool Chunk::isThisChunk(int x, int y, int z) const
{
	x -= this->x * chunkW;
	y -= this->y * chunkH;
	z -= this->z * chunkT;
	return x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT;
}

bool Chunk::isBlockAt(int x, int y, int z)
{
	int bx = getBlockX(x);
	int by = y;
	int bz = getBlockZ(z);
	if (bx >= 0 && bx < chunkW && by >= 0 && by < chunkH && bz >= 0 && bz < chunkT)
	{
		return blocks[by][bx][bz];
	}
	return false;
}

void Chunk::save()
{
#ifdef noSave
	return;
#endif // noSave

	struct stat sb;
	if (stat(path.c_str(), &sb) != 0)
		_mkdir(path.c_str());
	int times = 0;
	int ID = 0;
	int b = 0;
	std::string pathFile = fileName(x, y, z);
	std::ofstream save(pathFile);
	if (!save.is_open())
	{
#ifdef DebugFailMode
		printf("[Fail]: Cant Create file [%s]\n", pathFile.c_str());
#endif
		return;
	}

	nlohmann::json json;

	forAllBlocks
	{
		int IdNow = -1;
		if (blocks[j][i][k])
			IdNow = blocks[j][i][k]->getID();

		if (IdNow != ID)
		{
			if (times > 0)
			{
				json["Blocks"][b][0] = ID;
				json["Blocks"][b][1] = times;
				b++;
			}
			ID = IdNow;
			times = 1;
			continue;
		}
		else
		{
			times++;
		}
	}
	json["Blocks"][b][0] = ID;
	json["Blocks"][b][1] = times;
	json["AvgTemperature"] = avgTemperature;

	save << json;
	save.close();

}

bool Chunk::loadGame()
{
	std::string pathFile = fileName(x, y, z);
	std::ifstream read(pathFile);
	if (!read.is_open())
	{
		read.close();
		return false;
	}

	nlohmann::json json;
	read >> json;
	read.close();
	return loadGame(json);
}

bool Chunk::loadGame(nlohmann::json json)
{
	if (!json.contains("Blocks") || json["Blocks"].size() <= 0)
		return false;
	int times = 0;
	int ID = 0;
	int b = 0;
	int chunkX = this->x * chunkW;
	int chunkZ = this->x * chunkW;
	forAllBlocks
	{
		if (times <= 0)
		{
			if (json["Blocks"].size() <= b)
				return true;
			ID = json["Blocks"][b][0];
			times = json["Blocks"][b][1];
			b++;
		}
		times--;
		if (ID < 0)
		{
			continue;
		}
		else
		{
			setBlockAt(createBlock(ID, chunkX + i, j, chunkZ + k), i,j,k);
		}
	}
	if (json.contains("AvgTemperature"))
		avgTemperature = json["AvgTemperature"];
	return true;
}

void Chunk::clearBlocks()
{
	forAllBlocks
	if (blocks[j][i][k])
	{
		delete blocks[j][i][k];
		blocks[j][i][k] = NULL;
	}
	wasCleared = true;
	for (auto a : toAdd)
	{
		delete a;
	}
	toAdd.clear();
	toDelete.clear();
	toUpdate.clear();
}

void Chunk::saveBlockToChunk(int x, int y, int z, int ID,int rotate,int variant)
{
	int cx=0;
	int cy=0;
	int cz=0;
	if (x >= 0)
	{
		cx = x / chunkW;
	}
	else
	{
		cx = x / chunkW -1;
	}
	if (y >= 0)
	{
		cy = y / chunkH;
	}
	else
	{
		cy = y / chunkH - 1;
	}
	if (z >= 0)
	{
		cz = z / chunkT;
	}
	else
	{
		cz = z / chunkT - 1;
	}
	SaveChunkData* saveData = getChunkData(cx, cy, cz);
	if(!saveData)
	{
		SaveChunkData* data = new SaveChunkData();
		data->x = cx;
		data->y = cy;
		data->z = cz;
		std::ifstream reader;
		reader.open(fileName(cx, cy, cz));
		if (reader.is_open())
		{
			reader >> data->j;
			reader.close();
		}
		saveData = data;
		Chunk::data.push_back(data);
	}

	int n = 0;
	if (saveData->j.contains("ToAddBlock"))
	{
		n = saveData->j["ToAddBlock"].size();
	}

	nlohmann::json j;
	saveBlockToJson(j, x, y, z, ID, rotate, variant);
	saveData->j["ToAddBlock"][n] = j;
}

void Chunk::reloadBlocksFront()
{
	const int z = chunkT - 1;
	for (int i = 0; i < chunkH; i++)
	{
		for (int j = 0; j < chunkW; j++)
		{
			if (!blocks[i][j][z])
				continue;
			blocks[i][j][z]->setOneFace((int)Faces::Front, true);
		}
	}
	genVertices = true;
}

void Chunk::reloadBlocksBack()
{

	for (int i = 0; i < chunkH; i++)
	{
		for (int j = 0; j < chunkW; j++)
		{
			if (!blocks[i][j][0])
				continue;
			blocks[i][j][0]->setOneFace((int)Faces::Back, true);
		}
	}
	genVertices = true;
}

void Chunk::reloadBlocksLeft()
{
	for (int i = 0; i < chunkH; i++)
	{
		for (int j = 0; j < chunkT; j++)
		{
			if (!blocks[i][0][j])
				continue;
			blocks[i][0][j]->setOneFace((int)Faces::Left, true);
		}
	}
	genVertices = true;
}

void Chunk::reloadBlocksRight()
{
	const int x = chunkW - 1;
	for (int i = 0; i < chunkH; i++)
	{
		for (int j = 0; j < chunkT; j++)
		{
			if (!blocks[i][x][j])
				continue;
			blocks[i][x][j]->setOneFace((int)Faces::Right, true);
		}
	}
	genVertices = true;
}

void Chunk::genVerticPos(int dir)
{
	std::vector<glm::uvec2> vertices;
	int sizeX = 0;
	int sizeY = 0;
	glm::uvec2 data = glm::uvec2(0, 0);
	const int firstSize = 63;
	const int secendSize = 31;
	if (dir <= 1)
	{
		int blocksID[chunkH][chunkT];
		for (int i = 0; i < chunkW; i++)
		{
			for (int j = 0; j < chunkH; j++)
				for (int k = 0; k < chunkT; k++)
				{
					if (blocks[j][i][k] && blocks[j][i][k]->isRenderedSide(dir))
						blocksID[j][k] = blocks[j][i][k]->getID();
					else
						blocksID[j][k] = -1;
				}
			for (int k = 0; k < chunkT; k++)
				for (int j = 0; j < chunkH; j++)
				{
					if (blocksID[j][k] < 0)
						continue;
					data = blocks[j][i][k]->getVertex(dir);


					for (sizeX = 0; sizeX < firstSize && j + sizeX + 1 < chunkH; sizeX++)
					{
						if (blocksID[j + sizeX + 1][k] != blocksID[j][k])
						{
							break;
						}
						blocksID[j + sizeX + 1][k] = -1;
					}
					for (sizeY = 0; sizeY < secendSize && k + sizeY + 1 < chunkT; sizeY++)
					{
						bool breaked = false;
						for (int s = 0; s <= sizeX; s++)
							if (blocksID[j + s][k + sizeY + 1] != blocksID[j][k])
							{
								breaked = true;
								break;
							}

						if (breaked)
							break;
						else
							for (int s = 0; s <= sizeX; s++)
								blocksID[j + s][k + sizeY + 1] = -1;
					}
					blocksID[j][k] = -1;
					data.y += (sizeX << 21) + (sizeY << 27);

					vertices.push_back(data);
					j += sizeX;
				}

		}
		
	}
	else if (dir <= 3)
	{
		int blocksID[chunkH][chunkW];
		for (int k = 0; k < chunkT; k++)
		{
			for (int j = 0; j < chunkH; j++)
				for (int i = 0; i < chunkW; i++)
				{
					if (blocks[j][i][k] && blocks[j][i][k]->isRenderedSide(dir))
						blocksID[j][i] = blocks[j][i][k]->getID();
					else
						blocksID[j][i] = -1;
				}
			for (int i = 0; i < chunkW; i++)
				for (int j = 0; j < chunkH; j++)
				{
					if (blocksID[j][i] < 0)
						continue;

					data = blocks[j][i][k]->getVertex(dir);
					for (sizeX = 0; sizeX < firstSize && j + sizeX + 1 < chunkH; sizeX++)
					{
						if (blocksID[j + sizeX + 1][i] != blocksID[j][i])
						{
							break;
						}
						blocksID[j + sizeX + 1][i] = -1;
					}
					for (sizeY = 0; sizeY < secendSize && i + sizeY + 1 < chunkW; sizeY++)
					{
						bool breaked = false;
						for (int s = 0; s <= sizeX; s++)
							if (blocksID[j + s][i + sizeY + 1] != blocksID[j][i])
							{
								breaked = true;
								break;
							}

						if (breaked)
							break;
						else
							for (int s = 0; s <= sizeX; s++)
								blocksID[j + s][i + sizeY + 1] = -1;
					}
					blocksID[j][i] = -1;
					data.y += (sizeX << 21) + (sizeY << 27);
					vertices.push_back(data);
					j += sizeX;
				}
		}


	}
	else if (dir <= 5)
	{
		int blocksID[chunkW][chunkT];

		for (int j = 0; j < chunkH; j++)
		{
			for (int k = 0; k < chunkT; k++)
				for (int i = 0; i < chunkW; i++)
				{
					if (blocks[j][i][k] && blocks[j][i][k]->isRenderedSide(dir))
						blocksID[i][k] = blocks[j][i][k]->getID();
					else
						blocksID[i][k] = -1;
				}
			for (int k = 0; k < chunkT; k++)
				for (int i = 0; i < chunkW; i++)
				{
					if (blocksID[i][k] < 0)
						continue;
					for (sizeX = 0; sizeX < firstSize && i + sizeX + 1 < chunkW; sizeX++)
					{
						if (blocksID[i + sizeX + 1][k] != blocksID[i][k])
							break;
						blocksID[i + sizeX + 1][k] = -1;
					}
					for (sizeY = 0; sizeY < secendSize && k + sizeY + 1 < chunkT; sizeY++)
					{
						bool breaked = false;
						for (int s = 0; s <= sizeX; s++)
							if (blocksID[i + s][k + sizeY + 1] != blocksID[i][k])
							{
								breaked = true;
								break;
							}

						if (breaked)
							break;
						else
							for (int s = 0; s <= sizeX; s++)
								blocksID[i + s][k + sizeY + 1] = -1;


					}
					data = blocks[j][i][k]->getVertex(dir);
					data.y += (sizeX << 21) + (sizeY << 27);
					vertices.push_back(data);
					blocksID[i][k] = -1;
					i += sizeX;
				}
		}

	}
	else
	{
		int blocksID[chunkH];
		for (int k = 0; k < chunkT; k++)
			for (int i = 0; i < chunkW; i++)
			{
				for (int j = 0; j < chunkH; j++)
				{
					if (blocks[j][i][k] && blocks[j][i][k]->isRenderedSide(dir))
						blocksID[j] = blocks[j][i][k]->getID();
					else
						blocksID[j] = -1;
				}
				for (int j = 0; j < chunkH; j++)
				{
					if (blocksID[j] < 0)
						continue;

					data = blocks[j][i][k]->getVertex(dir);

					for (sizeX = 0; sizeX < firstSize && j + sizeX + 1 < chunkH; sizeX++)
					{
						if (blocksID[j + sizeX + 1] != blocksID[j])
							break;
						blocksID[j + sizeX + 1] = -1;
					}

					blocksID[j] = -1;
					data.y += (sizeX << 21);
					vertices.push_back(data);
					j += sizeX;
				}
				
			}
	}
	mesh[dir]->clearMesh();
	mesh[dir]->addData(vertices);
}

void Chunk::genVerticesPos()
{

	const int vecSize = 10;

	std::thread worker[vecSize];
	for (int i = 0; i < vecSize; i++)
	{
		worker[i] = std::thread(&Chunk::genVerticPos, this, i);
	}
	for (int i = 0; i < vecSize; i++)
		worker[i].join();
	return;
	
}

float getValue(float v)
{
	if (v <= -1.0f)
		return -log(-v) / 6.0f - 1;
	if (v >= 1.0f)
		return log(v) / 6.0f + 1;
	return powf(v, 3);
}

void genValues(float** tab)
{
	int w = chunkW - 1;
	int t = chunkT - 1;
	for (int x = 1; x < w; x++)
	{
		float v = (float)x / (float)chunkW;
		tab[x][0] = (1.0f - v) * tab[0][0] + v * tab[w][0];
		tab[x][t] = (1.0f - v) * tab[0][t] + v * tab[w][t];

	}
	for (int z = 1; z < t; z++)
	{
		float v = (float)z / (float)chunkT;
		tab[0][z] = (1.0f - v) * tab[0][0] + v * tab[0][t];
		tab[w][z] = (1.0f - v) * tab[w][0] + v * tab[w][t];
	}
	for (int x = 1; x < w; x++)
		for (int z = 1; z < t; z++)
		{
			float vx = (float)x / (float)chunkW;
			float vz = (float)z / (float)chunkT;
			tab[x][z] = ((1.0f - vx) * tab[0][z] + vx * tab[w][z] + (1.0f - vz) * tab[x][0] + vz * tab[x][t]) / 2;
		}
}

void Chunk::saveBlockToJson(nlohmann::json& j, int& x, int& y, int& z, int& ID, int& rotate, int& variant)
{
	j[0] = ID;
	j[1] = x;
	j[2] = y;
	j[3] = z;
	j[4] = rotate;
	j[5] = variant;
}

void Chunk::readBlockToJson(nlohmann::json& j, int& x, int& y, int& z, int& ID, int& rotate, int& variant)
{
	ID = j[0];
	x = j[1];
	y = j[2];
	z = j[3];
	rotate = j[4];
	variant = j[5];
}

void Chunk::saveBlockData()
{
	std::ofstream s;
	for (auto d : data)
	{
		s.open(fileName(d->x, d->y, d->z));
		if (s.is_open())
		{
			s << d->j;
			s.close();
		}
		delete d;
	}
	data.clear();
}

bool Chunk::hasDataToRead(int x, int y, int z)
{
	return getChunkData(x, y, z);
}

SaveChunkData* Chunk::getChunkData(int x, int y, int z)
{
	for (auto d : data)
		if (x == d->x && y == d->y && z == d->z)
			return d;
	return NULL;
}

float getValueTerrain(float v)
{
	if (v <= -1.0f)
		return -log(-v) / 6.0f - 1;
	if (v >= 1.0f)
		return log(v) / 6.0f + 1;
	return v;
}

void Chunk::genStone(int &x, int &z, int h)
{
	int blockX = x + this->x * (chunkW);
	int blockZ = z + this->z * (chunkT);
	for (int i = 0; i < h && i < chunkH; i++)
	{
		if (blocks[i][x][z])
			delete blocks[i][x][z];
		setBlockAt(createBlock(2, blockX, i, blockZ), x, i, z);
	}
}

void Chunk::fillWater(int &x,int &z,int h,float &temperature)
{
	int blockX = x + this->x * (chunkW);
	int blockZ = z + this->z * (chunkT);
	if (h < 0)
		h = 0;
	for (int j = h; j < waterH && j < chunkH; j++)
	{
		if (!blocks[j][x][z])
			setBlockAt(createBlock(11, blockX, j, blockZ), x, j, z);
	}
	if (!blocks[waterH][x][z])
		setBlockAt(createBlock((temperature <= -0.3) ? 20 : 11, blockX, waterH, blockZ), x, waterH, z);

}

void Chunk::genSandForWater(int &x, int &z,int y, int h)
{
	int blockX = x + this->x * (chunkW);
	int blockZ = z + this->z * (chunkT);
	if (y < 0)
		y = 0;
	for (int j = y; j <= h && j <= waterH; j++)
	{
		if (blocks[j][x][z])
			delete blocks[j][x][z];
		setBlockAt(createBlock(4, blockX, j, blockZ), x, j, z);
	}
}

void Chunk::biomLayer(int &x, int &z, int y, int h, float &temperature, float &structureNoise)
{
	if (y < 0)
		y = 0;
	int blockX = x + this->x * (chunkW);
	int blockZ = z + this->z * (chunkT);
	int blockID = 1;
	int blockSurfaceID = 0;
	if (temperature >= 0.3)
	{
		blockSurfaceID = 4;
		blockID = 4;
	}
	else if (temperature <= -0.3)
	{
		blockSurfaceID = 3;
	}
	for (int j = y; j <= h && j < chunkH; j++)
	{
		if (blocks[j][x][z])
			delete blocks[j][x][z];
		setBlockAt(createBlock(j == h ? blockSurfaceID : blockID, blockX, j, blockZ), x, j, z);
	}
}

void Chunk::setBlockAt(Block* b, int x, int y, int z)
{
	blocks[y][x][z] = b;
	if (!b)
		return;
	if (b->isUpdateBlock())
		toUpdate.push_back(b);
}

void Chunk::genPlants(int &x, int &z, int y, float &temperature, float &structureNoise)
{
	if (structureNoise > 0.85)
		return;
	if (y >= chunkH|| y <= waterH)
		return;
	int blockX = x + this->x * (chunkW);
	int blockZ = z + this->z * (chunkT);
	int div = pow(-structureNoise + 3, 2) * 50;
	if (div <= 20)
		div = 20;
	int value = abs(temperature) * 1000000;
	if (y == waterH + 1)
	{
		if (value % 169 == 0 && !blocks[y][x][z])
		{
			int ch = rand() % 4 + 1;
			for (int cy = 0; cy < ch; cy++)
			{
				if (!blocks[y + cy][x][z])
					setBlockAt(createBlock(22, blockX, y + cy, blockZ), x, y + cy, z);
			}
		}
		return;
	}
	if ((int)(value) % div == 0)
	{
		if (temperature < -0.3)
			setBlockAt(createBlock(14, blockX, y, blockZ), x, y, z);
		else if (temperature < 0.3)
		{
			if ((int)(value) % 10 >= 6)
				setBlockAt(createBlock(12, blockX, y, blockZ), x, y, z);
			else
				setBlockAt(createBlock(13, blockX, y, blockZ), x, y, z);

		}
		else
		{
			if ((int)(value) % 69 == 0)
			{
				int ch = rand() % 4;
				for (int cy = 0; cy < ch && y + cy < chunkH; cy++)
				{
					if (!blocks[y + cy][x][z])
						setBlockAt(createBlock(15, blockX, y + cy, blockZ), x, y + cy, z);
				}
			}
		}

	}
	else if (temperature >= -0.3 && temperature <= 0.3 && ((int)(value) % 469) % 3 == 0)
	{
		int blockId = -1;
		if ((int)(value) % 100 == 10)
			blockId = 19;
		else if ((int)(value) % 100 == 69)
			blockId = 17;
		else if ((int)(value) % 100 == 21)
			blockId = 18;
		else if ((int)(value) % 25  == 0)
			blockId = 16;
		if (blockId > -1 && !blocks[y][x][z])
			setBlockAt(createBlock(blockId, blockX, y, blockZ), x, y, z);
	}
	else if (temperature < -0.3 && ((int)(value) % 100) == 0)
	{
		if (!blocks[y][x][z])
			setBlockAt(createBlock(21, blockX, y, blockZ), x, y, z);
	}
}

void Chunk::genStructures(int &x, int &z, int y, float &temperature, float &structureNoise)
{
	int blockX = x + this->x * (chunkW);
	int blockZ = z + this->z * (chunkT);
	int value = abs(temperature) * 1000000;
	if (((int)(value) % 4567) == 0)
	{
		if (y <= waterH)
			return;
		if (blocks[y][x][z])
			delete blocks[y][x][z];

		setBlockAt(createStructure(((value) % 162)%10+1, blockX, y, blockZ), x, y, z);

	}
}

int getMapHeight(float terain, float erozja, float pv, float river, float structure)
{
	int h = minH + (terain + erozja / 3 + pv / 18) * 18.0f / 25.0f * (maxH - minH);
	if (0.90f < river)
	{
		int rivDeep = (abs(0.90f - river) * 10.0f) * 8;
		if (h > waterH - rivDeep)
		{
			h = waterH - rivDeep;
		}
	}
	else if (river > 0.70f && h >= waterH)
	{
		h = pow(1.0f - abs(0.70f - river) * 5, 2) * (h - waterH) + waterH;
		if (h <= waterH - 1)
			h = waterH;
	}
	if (h <= 0)
		return 0;

	if (structure >= 0.90)
	{
		return ViligeH;
	}
	if (structure >= 0.80)
	{
		float p = (0.90 - structure);
		return ViligeH * (1 - p) + h * p;
	}
	if (structure >= 0.50)
	{
		float p = (0.80 - structure) * 10.0f / 3.0f + 0.1;
		if (p > 1)
			p = 1;
		return ViligeH * (1 - p) + h * p;
	}
	


	return h;
}

int getTereinH(FastNoiseLite& terrain, FastNoiseLite& erosion, FastNoiseLite& picksAndValues, FastNoiseLite& riverNoise, FastNoiseLite& structureNoise, float x, float z)
{
	float t = (getValueTerrain(terrain.GetNoise(x, z)) + 1) / 2;
	float e = getValueTerrain(erosion.GetNoise(x, z));
	float pv = getValueTerrain(picksAndValues.GetNoise(x, z));
	float riverV = riverNoise.GetNoise(x, z);
	float structureV = structureNoise.GetNoise(x, z);

	return getMapHeight(t, e, pv, riverV, structureV);
}

glm::vec2 getMaxViligeNoisePoint(FastNoiseLite& structureNoise, float x, float z)
{
	glm::vec2 maxP = { -1,-1 };
	float maxV = 0.0f;
	glm::ivec2 startPos(x, z);
	std::vector<glm::ivec2> points = { glm::ivec2(x,z) };
	std::vector<glm::ivec2> checkPoints;
	while (points.size() > 0)
	{
		glm::ivec2 point = points.back();
		points.pop_back();

		float structureV = structureNoise.GetNoise((float)point.x, (float)point.y);
		if (structureV > 0.90f && structureV > maxV)
		{
			auto it = std::find(checkPoints.begin(), checkPoints.end(), point + glm::ivec2(0, 1));
			auto it2 = std::find(points.begin(), points.end(), point + glm::ivec2(0, 1));
			if (it == checkPoints.end() && it2 == points.end())
				points.push_back(point + glm::ivec2(0, 1));
			it = std::find(checkPoints.begin(), checkPoints.end(), point + glm::ivec2(0, -1));
			it2 = std::find(points.begin(), points.end(), point + glm::ivec2(0, -1));
			if (it == checkPoints.end() && it2 == points.end())
				points.push_back(point + glm::ivec2(0, -1));
			it = std::find(checkPoints.begin(), checkPoints.end(), point + glm::ivec2(1, 0));
			it2 = std::find(points.begin(), points.end(), point + glm::ivec2(1, 0));
			if (it == checkPoints.end() && it2 == points.end())
				points.push_back(point + glm::ivec2(1, 0));
			it = std::find(checkPoints.begin(), checkPoints.end(), point + glm::ivec2(-1, 0));
			it2 = std::find(points.begin(), points.end(), point + glm::ivec2(-1, 0));
			if (it == checkPoints.end() && it2 == points.end())
				points.push_back(point + glm::ivec2(-1, 0));
			if (structureV > maxV)
			{
				maxP = point;
				maxV = structureV;

			}
			else if (structureV == maxV)
			{
				if(glm::distance(glm::vec2(x,z),maxP) > glm::distance(glm::vec2(x, z), glm::vec2(point)))
				{
					maxP = point;
					maxV = structureV;
				}
			}
		}


		checkPoints.push_back(point);
	}
	return glm::vec2(maxP.x, maxP.y);
}

int getAvgH(float x, float z, float w, float t,FastNoiseLite& terrain, FastNoiseLite& erosion, FastNoiseLite& picksAndValues, FastNoiseLite& riverNoise, FastNoiseLite& structureNoise)
{
	if (w <= 0 || t <= 0)
		return 0;
	int h = 0;
	for (int i = 0; i < w; i++)
		for (int j = 0; j < t; j++)
			h += getTereinH(terrain, erosion, picksAndValues, riverNoise, structureNoise, x + i, j + z);
	
	return h / (w * t);
}

void Chunk::generateTerrain()
{

	FastNoiseLite terrain(seed);
	FastNoiseLite erosion(seed);
	FastNoiseLite picksAndValues(seed);
	FastNoiseLite riverNoise(seed);
	FastNoiseLite temperatureNoise(seed);
	FastNoiseLite structureNoise(seed);
	{
		float multiplay = 0.3f;
		terrain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
		terrain.SetFrequency(0.001f * multiplay);
		terrain.SetFractalType(FastNoiseLite::FractalType_FBm);
		terrain.SetFractalOctaves(3);
		terrain.SetFractalLacunarity(2.0f);
		terrain.SetFractalGain(2.177f);
		terrain.SetFractalWeightedStrength(4.8f);
		erosion.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		erosion.SetFrequency(0.01f * multiplay);
		erosion.SetFractalType(FastNoiseLite::FractalType_FBm);
		erosion.SetFractalOctaves(3);
		erosion.SetFractalLacunarity(0.91f);
		erosion.SetFractalGain(1.34f);
		erosion.SetFractalWeightedStrength(4.64f);
		picksAndValues.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		picksAndValues.SetFrequency(0.001f * multiplay);
		picksAndValues.SetFractalType(FastNoiseLite::FractalType_FBm);
		picksAndValues.SetFractalOctaves(5);
		picksAndValues.SetFractalLacunarity(2.9f);
		picksAndValues.SetFractalGain(1.01f);
		picksAndValues.SetFractalWeightedStrength(2.560f);
		riverNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		riverNoise.SetFrequency(0.001f * multiplay);
		riverNoise.SetFractalType(FastNoiseLite::FractalType_Ridged);
		riverNoise.SetFractalOctaves(1);
		riverNoise.SetFractalLacunarity(1.7f);
		riverNoise.SetFractalGain(0.53f);
		riverNoise.SetFractalWeightedStrength(0.000f);
		temperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		temperatureNoise.SetFrequency(0.0005f * multiplay);
		temperatureNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
		temperatureNoise.SetFractalOctaves(3);
		temperatureNoise.SetFractalLacunarity(1.3f);
		temperatureNoise.SetFractalGain(0.32f);
		temperatureNoise.SetFractalWeightedStrength(6.16f);
		structureNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		structureNoise.SetFrequency(0.006f * multiplay);
		structureNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
		structureNoise.SetFractalOctaves(4);
		structureNoise.SetFractalLacunarity(0.95f);
		structureNoise.SetFractalGain(0.5f);
		structureNoise.SetFractalWeightedStrength(6.960f);
	}
	bool hasVilage = false;
	int viligeX = -1;
	int viligeZ = -1;
	float structurePower = 0.0f;
	const int height = maxH - minH;
	const int dirtSize = 8;
	avgTemperature = 0;
	for (int i = 0; i < chunkW; i++)
		for (int k = 0; k < chunkT; k++)
		{
			float x = i + this->x * (chunkW);
			float z = k + this->z * (chunkT);

			float temperatureV = temperatureNoise.GetNoise(x, z);
			float structureV = structureNoise.GetNoise(x, z);
			avgTemperature += temperatureV;
			int h = getTereinH(terrain, erosion, picksAndValues, riverNoise, structureNoise, x, z);
			if (structureV >= 0.9 && structureV > structurePower)
			{
				hasVilage = true;
				viligeX = x;
				viligeZ = z;
				structurePower = structureV;
			}

			genStone(i, k, h - 7);
			biomLayer(i, k, h - 7, h, temperatureV, structureV);
			genSandForWater(i, k, h - 3, h);
			fillWater(i, k, h, temperatureV);

			genPlants(i, k, h + 1, temperatureV, structureV);

		}
	avgTemperature /= chunkW * chunkT;
	if (!hasVilage)
		return;
	
	glm::vec2 pos = getMaxViligeNoisePoint(structureNoise, viligeX, viligeZ);
	int posX = pos.x - this->x * chunkW;
	int posZ = pos.y - this->z * chunkT;
	if (posX < 0 || posX >= chunkW || posZ < 0 || posZ >= chunkT)
		return;
	float r = 0;
	const float maxViligeValue = 0.7;
	for (r = 0;; r++)
	{
		float structureUp = structureNoise.GetNoise(pos.x, pos.y + r);
		float structureDown = structureNoise.GetNoise(pos.x, pos.y - r);
		float structureLeft = structureNoise.GetNoise(pos.x + r, pos.y);
		float structureRight = structureNoise.GetNoise(pos.x - r, pos.y);
		if (structureUp < maxViligeValue || structureDown < maxViligeValue || structureLeft < maxViligeValue || structureRight < maxViligeValue)
			break;
	}

	int range = r / StructureTileSize + 1;
	if (range <= 0)
		return;
	srand(seed + x + y + z);
	//srand(time(NULL));
	Tile** t = generateVillage(range);
	for (int x = 0; x < range; x++)
		for (int z = 0; z < range; z++)
		{
			if (t[x + z * range])
			{
				int viligex = pos.x + x * StructureTileSize - r / 2;
				int viligez = pos.y + z * StructureTileSize - r / 2;
				int viligey = getAvgH(viligex + StructureTileSize / 2 - 2, viligez + StructureTileSize / 2 - 2, 5, 5, terrain, erosion, picksAndValues, riverNoise, structureNoise);
				if (viligey < ViligeH-2)
					viligey = ViligeH-2;
				Tile* tile = t[x + z * range];
				StructureHandler* str = createStructure(tile->ID, viligex, viligey, viligez);
				if (!str)
					continue;
				if (avgTemperature > 0.3)
					str->setVariant(1);
				else if (avgTemperature < -0.3)
					str->setVariant(2);
				for (int i = 0; i < (tile->rotate) % 4; i++)
					str->rotate();
				game->deleteBlock(viligex, viligey, viligez);
				if (!game->addBlock(str))
					delete str;
			}
		}

	for (auto i = 0; i < range * range; i++)
		if (t[i])
			delete t[i];
	delete t;
	
}

int getBlockX(int x)
{
	if (x >= 0)
		return x % chunkW;
	return (chunkW - (abs(x) % chunkW)) % chunkW;


}

int getBlockZ(int z)
{
	if (z >= 0)
		return z % chunkT;
	return (chunkT - (abs(z) % chunkT)) % chunkT;
}

void Chunk::setFacing()
{
	Chunk* lChunk = game->getChunkAt(x - 1, y, z);
	Chunk* rChunk = game->getChunkAt(x + 1, y, z);
	Chunk* bChunk = game->getChunkAt(x, y, z - 1);
	Chunk* fChunk = game->getChunkAt(x, y, z + 1);
	forAllBlocks
	{
		if (!blocks[j][i][k])
			continue;
		if (j - 1 > 0  && blocks[j - 1][i][k])
		{
			blocks[j][i][k]->setOneFace((int)Faces::Down, blocks[j - 1][i][k]);
			blocks[j - 1][i][k]->setOneFace((int)Faces::Up, blocks[j][i][k]);
		}
		if (j + 1 < chunkH  && blocks[j + 1][i][k])
		{
			blocks[j][i][k]->setOneFace((int)Faces::Up, blocks[j + 1][i][k]);
			blocks[j + 1][i][k]->setOneFace((int)Faces::Down, blocks[j][i][k]);
		}
		if (i - 1 > 0  && blocks[j][i - 1][k])
		{
			blocks[j][i][k]->setOneFace((int)Faces::Left, blocks[j][i - 1][k]);
			blocks[j][i - 1][k]->setOneFace((int)Faces::Right, blocks[j][i][k]);
		}
		if (i + 1 < chunkW  && blocks[j][i + 1][k])
		{
			blocks[j][i][k]->setOneFace((int)Faces::Right, blocks[j][i + 1][k]);
			blocks[j][i + 1][k]->setOneFace((int)Faces::Left, blocks[j][i][k]);
		}
		if (k - 1 > 0  && blocks[j][i][k - 1])
		{
			blocks[j][i][k]->setOneFace((int)Faces::Back, blocks[j][i][k - 1]);
			blocks[j][i][k - 1]->setOneFace((int)Faces::Front, blocks[j][i][k]);
		}
		if (k + 1 < chunkT  && blocks[j][i][k + 1])
		{
			blocks[j][i][k]->setOneFace((int)Faces::Front, blocks[j][i][k + 1]);
			blocks[j][i][k + 1]->setOneFace((int)Faces::Back, blocks[j][i][k]);
		}
		if (i == 0 && blocks[j][i][k])
		{
			if (lChunk && lChunk->blocks[j][chunkW - 1][k])
			{
				Block* b1 = blocks[j][i][k];
				Block* b2 = lChunk->blocks[j][chunkW - 1][k];
				b1->setOneFace((int)Faces::Left, b2);
				b2->setOneFace((int)Faces::Right, b1);
			}
		} 
		else if (i == chunkW - 1 && blocks[j][i][k])
		{
			if (rChunk && rChunk->blocks[j][0][k])
			{
				Block* b1 = blocks[j][i][k];
				Block* b2 = rChunk->blocks[j][0][k];
				b1->setOneFace((int)Faces::Right, b2);
				b2->setOneFace((int)Faces::Left, b1);
			}
		}
		if (k == 0 && blocks[j][i][k])
		{
			if (bChunk && bChunk->blocks[j][i][chunkT - 1])
			{
				Block* b1 = blocks[j][i][k];
				Block* b2 = bChunk->blocks[j][i][chunkT - 1];
				b1->setOneFace((int)Faces::Back, b2);
				b2->setOneFace((int)Faces::Front, b1);
			}
		}
		else if (k == chunkT - 1 && blocks[j][i][k])
		{
			if (fChunk && fChunk->blocks[j][i][0])
			{
				Block* b1 = blocks[j][i][k];
				Block* b2 = fChunk->blocks[j][i][0];
				b1->setOneFace((int)Faces::Front, b2);
				b2->setOneFace((int)Faces::Back, b1);
			}
		}

	}
	if (lChunk)
		lChunk->genVerticesFlag();
	if (rChunk)
		rChunk->genVerticesFlag();
	if (fChunk)
		fChunk->genVerticesFlag();
	if (bChunk)
		bChunk->genVerticesFlag();
}

void Chunk::readDataFromDataFile(SaveChunkData* saveData)
{
	nlohmann::json j = saveData->j["ToAddBlock"];
	for (int i = 0; i < j.size(); i++)
	{
		int x, y, z, ID, rotate, variant;
		readBlockToJson(j[i], x, y, z, ID, rotate, variant);
		deleteBlock(x, y, z);
		
		if (ID < 0)
		{
			StructureHandler* structure = createStructure(-ID, x, y, z);
			if (structure)
			{
				for (int i = 0; i < rotate; i++)
					structure->rotate();
				structure->setVariant(variant);
				addBlock(structure);
			}

		}
		else
		{
			addBlock(createBlock(ID, x, y, z));
		}

	}
}
