#include "Chunk.h"
#include "CubeBlock.h"
#include "../scene/Game.h"
#include "FastNoiseLite.h"
#include "../core/VAO.h"
#include "../core/VBO.h"
#include "../core/EBO.h"
#include "BlocksCreator.h"
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

Chunk::Chunk(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	forAllBlocks
	{
		blocks[j][i][k] = NULL;
	}
	if(!loadGame())
		generateTeren();
	setFaceing();
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
	for (int i = 0; i < 6; i++)
	{
		delete solidMesh[i];
		delete transMesh[i];
	}
}

void Chunk::start()
{
	for (int i = 0; i < 6; i++)
	{
		solidMesh[i] = new ChunkMesh(i);
		transMesh[i] = new ChunkMesh(i);
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

	}


}

void Chunk::draw(Shader* s,bool trans)
{
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(x * chunkW, y * chunkH, z * chunkT));
	s->setUniformMat4(model, "model");
	for (int i = 0; i < 6; i++)
		solidMesh[i]->draw(s);
	if (trans)
		for (int i = 0; i < 6; i++)
			transMesh[i]->draw(s);
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
	std::string pathFile = path + "chunk " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ".json";
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
	int times = 0;
	int ID = 0;
	int b = 0;
	std::string pathFile = path + "chunk " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ".json";
	std::ifstream read(pathFile);
	if (!read.is_open())
	{
		read.close();
		return false;
	}

	nlohmann::json json;
	read >> json;
	read.close();
	if (!json.contains("Blocks") || json["Blocks"].size() <= 0)
		return false;
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

void Chunk::genVerticesPos()
{
	const int vecSize = 6;
	std::vector<int> verticesSolid[vecSize];
	std::vector<int> verticesTrans[vecSize];

	GLuint lastIndexSolid = 0;
	GLuint lastIndexTrans = 0;
	forAllBlocks
	if (blocks[j][i][k])
	{
		if (blocks[j][i][k]->indexSize() <= 0)
			continue;
		std::vector<int> vert;

		if (blocks[j][i][k]->isTransparent())
		{
			for (int w = 0; w < vecSize; w++)
			{
				if (blocks[j][i][k]->isRenderedSide(w))
				{
					verticesTrans[w].push_back(blocks[j][i][k]->getVertex(w));
				}

			}
		}
		else
		{

			for (int w = 0; w < vecSize; w++)
			{

				if (blocks[j][i][k]->isRenderedSide(w))
				{
					verticesSolid[w].push_back(blocks[j][i][k]->getVertex(w));
				}

			}
		}
		



	}
	for (int i = 0; i < vecSize; i++)
	{
		solidMesh[i]->newMesh(verticesSolid[i]);
		transMesh[i]->newMesh(verticesTrans[i]);
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
	terrain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	terrain.SetFrequency(0.001f);
	terrain.SetFractalType(FastNoiseLite::FractalType_FBm);
	terrain.SetFractalOctaves(3);
	terrain.SetFractalLacunarity(2.0f);
	terrain.SetFractalGain(2.177f);
	terrain.SetFractalWeightedStrength(4.8f);

	FastNoiseLite erosia(2137);
	erosia.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	erosia.SetFrequency(0.01f);
	erosia.SetFractalType(FastNoiseLite::FractalType_FBm);
	erosia.SetFractalOctaves(3);
	erosia.SetFractalLacunarity(0.91f);
	erosia.SetFractalGain(1.34f);
	erosia.SetFractalWeightedStrength(4.64f);

	FastNoiseLite picksAndValies(80085);
	picksAndValies.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	picksAndValies.SetFrequency(0.001f);
	picksAndValies.SetFractalType(FastNoiseLite::FractalType_FBm);
	picksAndValies.SetFractalOctaves(5);
	picksAndValies.SetFractalLacunarity(2.9f);
	picksAndValies.SetFractalGain(1.01f);
	picksAndValies.SetFractalWeightedStrength(2.560f);

	FastNoiseLite riverNoise(80085);
	riverNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	riverNoise.SetFrequency(0.001f);
	riverNoise.SetFractalType(FastNoiseLite::FractalType_Ridged);
	riverNoise.SetFractalOctaves(1);
	riverNoise.SetFractalLacunarity(1.7f);
	riverNoise.SetFractalGain(0.53f);
	riverNoise.SetFractalWeightedStrength(0.000f);

	FastNoiseLite temperatureNoise(80085);
	temperatureNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	temperatureNoise.SetFrequency(0.001f);
	temperatureNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
	temperatureNoise.SetFractalOctaves(3);
	temperatureNoise.SetFractalLacunarity(1.3f);
	temperatureNoise.SetFractalGain(0.32f);
	temperatureNoise.SetFractalWeightedStrength(6.16f);

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


			///RIVER
			float v = riverNoise.GetNoise(x, z);
			river = 0.90f < v;
			///RIVER WATER
			rivDeep = (abs(0.90f - v) * 10.0f) * 6;
			if (rivDeep >= 6)
				rivDeep = 6;
			///RIVER SAND
			if (v > 0.70f && !river && h >= waterH)
			{
				h = pow(1.0f-abs(0.70f - v) * 5, 2) * (h - waterH)+waterH;
				if (h <= waterH-1)
					h = waterH ;
			}
			///RIVER WATER LEVEL
			if (river && h > waterH)
			{
				h = waterH - rivDeep;
				if (h < 0)
					h = 0;
			}
			else
			{
				river = false;
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
			for (int j = endPos + 3; j <= waterH && j < chunkH; j++)
			{
				if (blocks[j][i][k])
					delete blocks[j][i][k];
				blocks[j][i][k] = createBlock(11, blockX, j, blockZ);

			}
			if (river && h>waterH)
			{
				int start = h - rivDeep-3;
				if (start < 0)
					start = 0;
				for (int j = start; j < h - rivDeep && j < chunkH; j++)
				{
					if (blocks[j][i][k])
						delete blocks[j][i][k];
					blocks[j][i][k] = createBlock(4, blockX, j, blockZ);

				}
				for (int j = h-rivDeep; j < h && j < chunkH; j++)
				{
					if (blocks[j][i][k])
						delete blocks[j][i][k];
					blocks[j][i][k] = createBlock(11, blockX, j, blockZ);

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
				if ((int)(picksAndValies.GetNoise(x, z) * 1000000) % 666 == 0)
				{
					if(temperatue<-0.3)
						blocks[j][i][k] = createBlock(14, blockX, j, blockZ);
					else if (temperatue < 0.3)
					{
						if((int)(picksAndValies.GetNoise(x, z) * 10000) % 10 >= 6)
							blocks[j][i][k] = createBlock(12, blockX, j, blockZ);
						else
							blocks[j][i][k] = createBlock(13, blockX, j, blockZ);
					}
					if (blocks[j][i][k])
						toUpdate.push_back(blocks[j][i][k]);

				}

			}

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
	return (chunkW - (abs(z) % chunkT)) % chunkT;
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
		const bool isTransparent = blocks[j][i][k]->isTransparent();
		bool trans;
		if (j - 1 > 0  && blocks[j - 1][i][k])
		{
			trans = blocks[j - 1][i][k]->isTransparent() != isTransparent;
			blocks[j][i][k]->setOneFace((int)Faces::Down, trans); 
			blocks[j - 1][i][k]->setOneFace((int)Faces::Up, trans);
		}
		if (j + 1 < chunkH  && blocks[j + 1][i][k])
		{
			trans = blocks[j + 1][i][k]->isTransparent() != isTransparent;
			blocks[j][i][k]->setOneFace((int)Faces::Up, trans);
			blocks[j + 1][i][k]->setOneFace((int)Faces::Down, trans);
		}
		if (i - 1 > 0  && blocks[j][i - 1][k])
		{
			trans = blocks[j][i - 1][k]->isTransparent() != isTransparent;
			blocks[j][i][k]->setOneFace((int)Faces::Left, trans);
			blocks[j][i - 1][k]->setOneFace((int)Faces::Right, trans);
		}
		if (i + 1 < chunkW  && blocks[j][i + 1][k])
		{
			trans = blocks[j][i + 1][k]->isTransparent() != isTransparent;
			blocks[j][i][k]->setOneFace((int)Faces::Right, trans);
			blocks[j][i + 1][k]->setOneFace((int)Faces::Left, trans);
		}
		if (k - 1 > 0  && blocks[j][i][k - 1])
		{
			trans = blocks[j][i][k - 1]->isTransparent() != isTransparent;
			blocks[j][i][k]->setOneFace((int)Faces::Back, trans);
			blocks[j][i][k - 1]->setOneFace((int)Faces::Front, trans);
		}
		if (k + 1 < chunkT  && blocks[j][i][k + 1])
		{
			trans = blocks[j][i][k + 1]->isTransparent() != isTransparent;
			blocks[j][i][k]->setOneFace((int)Faces::Front, trans);
			blocks[j][i][k + 1]->setOneFace((int)Faces::Back, trans);
		}
		if (i == 0 && blocks[j][i][k])
		{
			if (lChunk && lChunk->blocks[j][chunkW - 1][k])
			{
				Block* b1 = blocks[j][i][k];
				Block* b2 = lChunk->blocks[j][chunkW - 1][k];
				bool display = b1->getDisplay(b2);
				b1->setOneFace((int)Faces::Left, display);
				b2->setOneFace((int)Faces::Right, display);
			}
		} 
		else if (i == chunkW - 1 && blocks[j][i][k])
		{
			if (rChunk && rChunk->blocks[j][0][k])
			{
				Block* b1 = blocks[j][i][k];
				Block* b2 = rChunk->blocks[j][0][k];
				bool display = b1->getDisplay(b2);
				b1->setOneFace((int)Faces::Right, display);
				b2->setOneFace((int)Faces::Left, display);
			}
		}
		if (k == 0 && blocks[j][i][k])
		{
			if (bChunk && bChunk->blocks[j][i][chunkT - 1])
			{
				Block* b1 = blocks[j][i][k];
				Block* b2 = bChunk->blocks[j][i][chunkT - 1];
				bool display = b1->getDisplay(b2);
				b1->setOneFace((int)Faces::Back, display);
				b2->setOneFace((int)Faces::Front, display);
			}
		}
		else if (k == chunkT - 1 && blocks[j][i][k])
		{
			if (fChunk && fChunk->blocks[j][i][0])
			{
				Block* b1 = blocks[j][i][k];
				Block* b2 = fChunk->blocks[j][i][0];
				bool display = b1->getDisplay(b2);
				b1->setOneFace((int)Faces::Front, display);
				b2->setOneFace((int)Faces::Back, display);
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