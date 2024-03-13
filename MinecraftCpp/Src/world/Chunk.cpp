#include "Chunk.h"
#include "CubeBlock.h"
#include "../scene/Game.h"
#include "FastNoiseLite.h"

Game* Chunk::game = NULL;

Chunk::Chunk(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	for (int j = 0; j < chunkH; j++)
		for (int i = 0; i < chunkW; i++)
			for (int k = 0; k < chunkT; k++)
			{
				blocks[j][i][k] = NULL;
			}
	FastNoiseLite terrain(69);
	terrain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	terrain.SetFrequency(0.005f);
	terrain.SetFractalType(FastNoiseLite::FractalType_FBm);
	terrain.SetFractalOctaves(3);
	terrain.SetFractalGain(1.5f);
	terrain.SetFractalLacunarity(1.529f);
	terrain.SetFractalWeightedStrength(-0.2f);
	

	for (int i = 0; i < chunkW; i++)
		for (int k = 0; k < chunkT; k++)
		{
			int h = terrain.GetNoise((float)i + this->x * chunkW, (float)k + this->z * chunkT) * 32 - (this->y * chunkH);
			if (h > chunkH)
				h = chunkH;
			for (int j = 0; j < h; j++)
			{
				blocks[j][i][k] = new CubeBlock(i + x * chunkW, j + y * chunkH, k + z * chunkT, "Res/1.jpg");
				toAdd.push_back(blocks[j][i][k]);
			}
		}

}

Chunk::~Chunk()
{
	for (int j = 0; j < chunkH; j++)
		for (int i = 0; i < chunkW; i++)
			for (int k = 0; k < chunkT; k++)
				if (blocks[j][i][k])
					delete blocks[j][i][k];
}

void Chunk::update(float deltaTime)
{
	for (auto b : toAdd)
	{
		game->setFaceing(b, false);
		
	}
	toAdd.clear();
	for (auto b : toDelete)
	{
		game->setFaceing(b->x, b->y, b->z, true);
		delete b;
	}
	toDelete.clear();

}



void Chunk::draw()
{
	for (int y = 0; y < chunkH; y++)
		for (int x = 0; x < chunkW; x++)
			for (int z = 0; z < chunkT; z++)
				if(blocks[y][x][z])
					blocks[y][x][z]->draw();
}


Block* Chunk::getBlock(int x, int y, int z)
{
	x -= this->x * chunkW;
	y -= this->y * chunkH;
	z -= this->z * chunkT;
	if (x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT)
		return blocks[y][x][z];
	return NULL;
}

void Chunk::deleteBlock(Block* b)
{
	int x = b->x - this->x * chunkW;
	int y = b->y - this->y * chunkH;
	int z = b->z - this->z * chunkT;
	if (x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT)
	{
		toDelete.push_back(blocks[y][x][z]);
		blocks[y][x][z] = NULL;
	}
}

bool Chunk::isThisChunk(int x, int y, int z)
{
	x -= this->x * chunkW;
	y -= this->y * chunkH;
	z -= this->z * chunkT;
	return x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT;
}

	