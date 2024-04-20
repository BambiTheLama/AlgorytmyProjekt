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



#define forAllBlocks 	for (int j = 0; j < chunkH; j++)\
							for (int i = 0; i < chunkW; i++)\
								for (int k = 0; k < chunkT; k++)
Game* Chunk::game = NULL;
std::string Chunk::path = "World/";
std::vector<SaveChunkData*> Chunk::saveData;
static char**** blocksID = NULL;

Chunk::Chunk(int x, int y, int z)
{
	if (!blocksID)
	{
		blocksID = new char*** [chunkH];
		for (int i = 0; i < chunkH; i++)
		{
			blocksID[i] = new char** [chunkW];
			for (int j = 0; j < chunkW; j++)
			{
				blocksID[i][j] = new char* [chunkT];
				for (int k = 0; k < chunkW; k++)
				{
					blocksID[i][j][k] = new char[10];
				}
			}

		}
	}

	for (int i = 0; i < 10; i++)
	{
		mesh[i] = new ChunkMesh(i);
	}




	this->x = x;
	this->y = y;
	this->z = z;
	forAllBlocks
	{
		blocks[j][i][k] = NULL;
	}
	SaveChunkData* data = NULL;
	int element = -1;
	for (int i = 0; i < saveData.size(); i++)
		if (saveData[i]->x == x && saveData[i]->y == y && saveData[i]->z == z)
		{
			data = saveData[i];
			element = i;
			break;
		}
	if (data)
	{
		saveData.erase(saveData.begin() + element);
		if (!loadGame(data->j))
			generateTeren();
	}
	else
	{
		if (!loadGame())
			generateTeren();
	}
	setFaceing();


	if (!data)
		return;
	
	nlohmann::json j = data->j;

	if (j.contains("ToAdd"))
	{
		int size = j["ToAdd"].size();
		for (int i = 0; i < size; i++)
		{
			int ID = j["ToAdd"][i][0];
			int x = j["ToAdd"][i][1];
			int y = j["ToAdd"][i][2];
			int z = j["ToAdd"][i][3];
			Block* b = createBlock(ID, x, y, z);
			if (b)
			{
				if (!addBlock(b))
					delete b;
			}
		}

	}
	delete data;

}

Chunk::~Chunk()
{
	if (wasCleared)
		return;
	forAllBlocks
	if (blocks[j][i][k])
	{
		delete blocks[j][i][k];
	}

	for (int i = 0; i < 10; i++)
		delete mesh[i];
}

void Chunk::start()
{
	for (int i = 0; i < 10; i++)
	{
		mesh[i]->start();
		mesh[i]->genMesh();
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
		game->setFaceing(b->x, b->y, b->z, true);
		int toRemove = -1;
		for(int i=0;i<toUpdate.size();i++)
			if (b == toUpdate[i])
			{
				toRemove = i;
				break;
			}
		if (toRemove > -1)
			toUpdate.erase(toUpdate.begin() + toRemove);

		delete b;
	}
	for (auto b : toAdd)
	{
		if (b->faceToSetUp() <= 0)
			continue;
		game->setFaceing(b, b->faceToSetUp());
		int by = b->y;
		int bz = getBlockZ(b->z);
		int bx = getBlockX(b->x);
		if (!blocks[by][bx][bz])
			blocks[by][bx][bz] = b;
		
	}
	if (genVertices || toAdd.size() > 0 || toDelete.size() > 0)
	{
		genVertices = false;
		toDelete.clear();
		toAdd.clear();
		//setFaceing();
		genVerticesPos();
		for (int i = 0; i < 10; i++)
			mesh[i]->genMesh();

	}


}

void Chunk::draw(Shader* s)
{
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(x * chunkW, y * chunkH, z * chunkT));
	s->setUniformMat4(model, "model");

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
		for (auto d : toDelete)
			if (d == blocks[by][bx][bz])
				return;
		toDelete.push_back(blocks[by][bx][bz]);
	}
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

bool Chunk::isThisChunk(int x, int y, int z)
{
	x -= this->x * chunkW;
	y -= this->y * chunkH;
	z -= this->z * chunkT;
	return x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT;
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
			blocks[j][i][k] = createBlock(ID, i, j, k);
			if (blocks[j][i][k]->isUpdateBlock())
				toUpdate.push_back(blocks[j][i][k]);
		}
	}

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

void Chunk::saveBlockToChunk(int x, int y, int z, int ID)
{
	int cX;
	if (x >= 0)
		cX = x / chunkW;
	else
		cX = ((x) / chunkW )- 1;
	int cY = 0;
	int cZ;
	if (z >= 0)
		cZ = z / chunkT;
	else
		cZ = z / chunkT - 1;
	SaveChunkData* data = NULL;
	for (auto s : saveData)
		if (s->x == cX && s->y == cY && s->z == cZ)
		{
			data = s;
			break;
		}
	if (data == NULL)
	{
		data = new SaveChunkData();
		saveData.push_back(data);
		data->x = cX;
		data->y = cY;
		data->z = cZ;
		std::string pathFile = fileName(cX, cY, cZ);
		std::ifstream read(pathFile);
		if (read.is_open())
		{
			read >> data->j;
		}

		read.close();
	}
	
	int size = 0;
	if (data->j.contains("ToAdd"))
	{
		size = (int)data->j["ToAdd"].size();
	}
	data->j["ToAdd"][size][0] = ID;
	data->j["ToAdd"][size][1] = getBlockX(x);
	data->j["ToAdd"][size][2] = y;
	data->j["ToAdd"][size][3] = getBlockZ(z);

}

void Chunk::genVerticesPos()
{
	const int vecSize = 10;
	std::vector<int> vertices[vecSize];

	GLuint lastIndexSolid = 0;
	GLuint lastIndexTrans = 0;
	
	int sizeX = 0;
	int sizeY = 0;
	int data = 0;



	forAllBlocks
	{
		if (blocks[j][i][k])
		{
			for (int b = 0; b < 10; b++)
			{
				if (blocks[j][i][k]->isRenderedSide(b))
					blocksID[j][i][k][b] = blocks[j][i][k]->getID();
				else
					blocksID[j][i][k][b] = -1;
			}

		}
		else
		{
			for (int b = 0; b < 10; b++)
				blocksID[j][i][k][b] = -1;
		}
	}

	forAllBlocks
	{
		for (int b = 0; b < 2; b++)
		{
			if (blocksID[j][i][k][b] >= 0)
			{
				data = blocks[j][i][k]->getVertex(b);
				for (sizeX = 0; sizeX < 7 && j + sizeX + 1 < chunkH; sizeX++)
				{
					if (blocksID[j + sizeX + 1][i][k][b] != blocksID[j][i][k][b])
					{
						break;
					}
					blocksID[j + sizeX + 1][i][k][b] = -1;
				}
				for (sizeY = 0; sizeY < 7 && k + sizeY + 1 < chunkT; sizeY++)
				{
					bool breaked = false;
					for (int s = 0; s <= sizeX; s++)
						if (blocksID[j + s][i][k + sizeY + 1][b] != blocksID[j][i][k][b])
						{
							breaked = true;
							break;
						}

					if (breaked)
						break;
					else
						for (int s = 0; s <= sizeX; s++)
							blocksID[j + s][i][k + sizeY + 1][b] = -1;
				}
				blocksID[j][i][k][b] = -1;
				data += (sizeX << 26) + (sizeY << 29);

				vertices[b].push_back(data);

			}
		}
	for (int b = 2; b < 4; b++)
	{
		if (blocksID[j][i][k][b] >= 0)
		{
			data = blocks[j][i][k]->getVertex(b);
			for (sizeX = 0; sizeX < 7 && j + sizeX + 1 < chunkH; sizeX++)
			{
				if (blocksID[j + sizeX + 1][i][k][b] != blocksID[j][i][k][b])
				{
					break;
				}
				blocksID[j + sizeX + 1][i][k][b] = -1;
			}
			for (sizeY = 0; sizeY < 7 && i + sizeY + 1 < chunkW; sizeY++)
			{
				bool breaked = false;
				for (int s = 0; s <= sizeX; s++)
					if (blocksID[j + s][i + sizeY + 1][k][b] != blocksID[j][i][k][b])
					{
						breaked = true;
						break;
					}

				if (breaked)
					break;
				else
					for (int s = 0; s <= sizeX; s++)
						blocksID[j + s][i + sizeY + 1][k][b] = -1;
			}
			blocksID[j][i][k][b] = -1;
			data += (sizeX << 26) + (sizeY << 29);
			vertices[b].push_back(data);
		}
		for (int b = 4; b < 6; b++)
		{
			if (blocksID[j][i][k][b] >= 0)
			{

				for (sizeX = 0; sizeX < 7 && i + sizeX + 1 < chunkW; sizeX++)
				{
					if (blocksID[j][i + sizeX + 1][k][b] != blocksID[j][i][k][b])
						break;
					blocksID[j][i + sizeX + 1][k][b] = -1;
				}
				for (sizeY = 0; sizeY < 7 && k + sizeY + 1 < chunkT; sizeY++)
				{
					bool breaked = false;
					for (int s = 0; s <= sizeX; s++)
						if (blocksID[j][i + s][k + sizeY + 1][b] != blocksID[j][i][k][b])
						{
							breaked = true;
							break;
						}

					if (breaked)
						break;
					else
						for (int s = 0; s <= sizeX; s++)
							blocksID[j][i + s][k + sizeY + 1][b] = -1;


				}
				blocksID[j][i][k][b] = -1;

				data = blocks[j][i][k]->getVertex(b) + (sizeX << 26) + (sizeY << 29);
				vertices[b].push_back(data);
			}
		}
		for (int b = 6; b < 10; b++)
		{
			if (blocksID[j][i][k][b] >= 0)
			{
				data = blocks[j][i][k]->getVertex(b);

				for (sizeX = 0; sizeX < 8 && j + sizeX + 1 < chunkH; sizeX++)
				{
					if (blocksID[j + sizeX + 1][i][k][b] != blocksID[j][i][k][b])
						break;
					blocksID[j + sizeX + 1][i][k][b] = -1;
				}


				blocksID[j][i][k][b] = -1;
				data += (sizeX << 26);

				vertices[b].push_back(data);

			}
		}
	}
	}

	for (int i = 0; i < vecSize; i++)
	{
		mesh[i]->clearMesh();
		mesh[i]->addData(vertices[i]);
	}

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

void Chunk::saveBlockData()
{
	for (auto s : saveData)
	{
		std::string path = fileName(s->x, s->y, s->z);
		std::ofstream save(path);
		if (save.is_open())
		{
			save << s->j;
			save.close();
		}
		delete s;
		
	}
	saveData.clear();
}

float getValueTerrain(float v)
{
	if (v <= -1.0f)
		return -log(-v) / 6.0f - 1;
	if (v >= 1.0f)
		return log(v) / 6.0f + 1;
	return powf(v, 3);
}

void Chunk::generateTeren()
{

	FastNoiseLite terrain(666);
	FastNoiseLite erosia(2137);
	FastNoiseLite picksAndValies(80085);
	FastNoiseLite riverNoise(80085);
	FastNoiseLite temperatureNoise(80085);
	FastNoiseLite treesNoise(80085);
	{
		float multiplay = 0.5f;
		terrain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
		terrain.SetFrequency(0.001f * multiplay);
		terrain.SetFractalType(FastNoiseLite::FractalType_FBm);
		terrain.SetFractalOctaves(3);
		terrain.SetFractalLacunarity(2.0f);
		terrain.SetFractalGain(2.177f);
		terrain.SetFractalWeightedStrength(4.8f);
		erosia.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		erosia.SetFrequency(0.01f * multiplay);
		erosia.SetFractalType(FastNoiseLite::FractalType_FBm);
		erosia.SetFractalOctaves(3);
		erosia.SetFractalLacunarity(0.91f);
		erosia.SetFractalGain(1.34f);
		erosia.SetFractalWeightedStrength(4.64f);
		picksAndValies.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		picksAndValies.SetFrequency(0.001f * multiplay);
		picksAndValies.SetFractalType(FastNoiseLite::FractalType_FBm);
		picksAndValies.SetFractalOctaves(5);
		picksAndValies.SetFractalLacunarity(2.9f);
		picksAndValies.SetFractalGain(1.01f);
		picksAndValies.SetFractalWeightedStrength(2.560f);
		riverNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		riverNoise.SetFrequency(0.001f * multiplay);
		riverNoise.SetFractalType(FastNoiseLite::FractalType_Ridged);
		riverNoise.SetFractalOctaves(1);
		riverNoise.SetFractalLacunarity(1.7f);
		riverNoise.SetFractalGain(0.53f);
		riverNoise.SetFractalWeightedStrength(0.000f);
		temperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		temperatureNoise.SetFrequency(0.001f * multiplay);
		temperatureNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
		temperatureNoise.SetFractalOctaves(3);
		temperatureNoise.SetFractalLacunarity(1.3f);
		temperatureNoise.SetFractalGain(0.32f);
		temperatureNoise.SetFractalWeightedStrength(6.16f);
		treesNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		treesNoise.SetFrequency(0.001f * multiplay);
		treesNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
		treesNoise.SetFractalOctaves(4);
		treesNoise.SetFractalLacunarity(1.3f);
		treesNoise.SetFractalGain(0.5f);
		treesNoise.SetFractalWeightedStrength(9.0f);
	}

	const int height = maxH - minH;
	const int dirtSize = 8;
	for (int i = 0; i < chunkW; i++)
		for (int k = 0; k < chunkT; k++)
		{
			float x = i + this->x * (chunkW);
			float z = k + this->z * (chunkT);
			bool river = false;
			bool lake = false;
			int lakeDeep = waterH;
			int rivDeep = 0;

			float t = (getValueTerrain(terrain.GetNoise(x, z)) + 1)/2;
			float e = getValueTerrain(erosia.GetNoise(x, z));
			float pv = getValueTerrain(picksAndValies.GetNoise(x, z));
			float terrainV = (t + e / 3 + pv / 18) * 18.0f / 25.0f;
			int h = minH + terrainV * height;

			if (h < 0)
				h = 0;
			else if (h >= chunkH)
				h = chunkH-1;
			///RIVER
			float v = riverNoise.GetNoise(x, z);
			river = 0.90f < v;
			///RIVER WATER
			rivDeep = (abs(0.90f - v) * 10.0f) * 9;
			if (rivDeep > 9)
				rivDeep = 9;
			///RIVER SAND
			if (v > 0.70f && !river && h >= waterH)
			{
				h = pow(1.0f-abs(0.70f - v) * 5, 2) * (h - waterH)+waterH;
				if (h <= waterH-1)
					h = waterH ;
			}
			///RIVER WATER LEVEL
			if (river)
			{
				if (h >= waterH - rivDeep)
					h = waterH - rivDeep;
				if (h < 0)
					h = 0;
			}



			const int blockX = x;
			const int blockZ = z;

			if (h <= 2)
				h = 2;
			for (int j = 0; j < chunkH && j < h - dirtSize; j++)
			{
				if (blocks[j][i][k])
					delete blocks[j][i][k];
				blocks[j][i][k] = createBlock(2, blockX, j, blockZ);
			}
			int dirtStart = h - dirtSize;
			if (dirtStart < 0)
				dirtStart = 0;
			int endPos = h;
			if (endPos < waterH)
				endPos--;
			float temperatue = temperatureNoise.GetNoise(x, z);
			if (h > maxH * 0.57f)
				temperatue -= (float)h / maxH * 0.4;
			genBiom(i,k,x, z, dirtStart, endPos, temperatue);

			for (int j = endPos; j < endPos + 3 && j <= waterH && j < chunkH; j++)
			{
				if (blocks[j][i][k])
					delete blocks[j][i][k];
				blocks[j][i][k] = createBlock(4, blockX, j, blockZ);

			}

			if (river)
			{
				int start;
				if (h >= waterH)
					start = waterH - rivDeep - 3;
				else
					start = h - rivDeep - 3;

				if (start < 0)
					start = 0;
				for (int j = start; j < h - rivDeep && j < chunkH; j++)
				{
					if (blocks[j][i][k])
						delete blocks[j][i][k];
					blocks[j][i][k] = createBlock(4, blockX, j, blockZ);

				}
				if (h == waterH && (int)(picksAndValies.GetNoise(x, z) * 1000000) % 669 <= 3)
				{
					int n = (int)(blockX + blockZ) % 4 + 2;
					for (int j = 1; j < n; j++)
						if (!blocks[h + j][i][k])
							blocks[h + j][i][k] = createBlock(22, blockX, h + j, blockZ);
				}
			}
			if (lake)
			{
				for (int j = lakeDeep; j <= h && j < chunkH; j++)
				{
					if (blocks[j][i][k])
						delete blocks[j][i][k];
					blocks[j][i][k] = createBlock(11, blockX, j, blockZ);
				}

			}

			if (!lake && !river && h > waterH)
			{
				int j = h;

 				if (blocks[j][i][k])
					delete blocks[j][i][k];
				float treeV = treesNoise.GetNoise(x, z);
				if (treeV >= 3)
					treeV = 3;

				int div = pow(-treeV + 3, 2)*50;
				if (div <= 20)
					div = 20;

				if ((int)(picksAndValies.GetNoise(x, z) * 1000000) % div == 0)
				{
					if (temperatue < -0.3)
						blocks[j][i][k] = createBlock(14, blockX, j, blockZ);
					else if (temperatue < 0.3)
					{
						if ((int)(picksAndValies.GetNoise(x, z) * 10000000) % 10 >= 6)
							blocks[j][i][k] = createBlock(12, blockX, j, blockZ);
						else
							blocks[j][i][k] = createBlock(13, blockX, j, blockZ);
					}
					else
					{
						if ((int)(picksAndValies.GetNoise(x, z) * 1000000) % 69 == 0)
						{
							int ch = rand() % 4;
							for (int cy = 0; cy < ch; cy++)
							{
								if (!blocks[j + cy][i][k])
									blocks[j + cy][i][k] = createBlock(15, blockX, j + cy, blockZ);
							}

						}

					}
					if (blocks[j][i][k])
						toUpdate.push_back(blocks[j][i][k]);

				}
				else if (temperatue >= -0.3 && temperatue <= 0.3 && ((int)(picksAndValies.GetNoise(x, z) * 1000000) % 469) % 3 == 0)
				{
					int blockId = -1;
					if ((int)(picksAndValies.GetNoise(x, z) * 10000) % 100 == 10)
						blockId = 19;
					else if ((int)(picksAndValies.GetNoise(x, z) * 10000) % 100 == 69)
						blockId = 17;
					else if ((int)(picksAndValies.GetNoise(x, z) * 10000) % 100 == 21)
						blockId = 18;
					else if ((int)(picksAndValies.GetNoise(x, z) * 10000) % 100 % 2 == 0)
						blockId = 16;
					if (blockId > -1 && !blocks[j][i][k])
						blocks[j][i][k] = createBlock(blockId, blockX, j, blockZ);
				}
				else if (temperatue < -0.3 && ((int)(picksAndValies.GetNoise(x, z) * 1000000) % 100) == 0)
				{
					if (!blocks[j][i][k])
						blocks[j][i][k] = createBlock(21, blockX, j, blockZ);
				}


			}
			for (int j = h; j < waterH && j < chunkH; j++)
			{
				if (!blocks[j][i][k])
					blocks[j][i][k] = createBlock(11, blockX, j, blockZ);
			}
			if (!blocks[waterH][i][k])
				blocks[waterH][i][k] = createBlock((temperatue<=-0.3)?20:11, blockX, waterH, blockZ);
		}

}

void Chunk::genBiom(int x, int z,int blockX,int blockZ, int startY, int endY, float temperature)
{
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

	
	for (int j = startY; j < endY && j < chunkH; j++)
	{
		if (blocks[j][x][z])
			delete blocks[j][x][z];
		blocks[j][x][z] = createBlock(j == endY - 1 ? blockSurfaceID : blockID, blockX, j, blockZ);


	}
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

void Chunk::setFaceing()
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