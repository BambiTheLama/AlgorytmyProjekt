#include "Chunk.h"
#include "CubeBlock.h"
#include "../scene/Game.h"

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
				blocks[j][i][k] = new CubeBlock(i + x * chunkW, j + y * chunkH, k + z * chunkT, "Res/test1.png");
				toAdd.push_back(blocks[j][i][k]);
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
		if (!b)
			continue;
		Block* block = game->getBlockAt(b->x, b->y + 1, b->z);
		if (block)
		{
			b->setOneFace((int)Faces::Up, false);
			block->setOneFace((int)Faces::Down, false);
		}
		block = game->getBlockAt(b->x, b->y - 1, b->z);
		if (block)
		{
			block->setOneFace((int)Faces::Up, false);
			b->setOneFace((int)Faces::Down, false);
		}
		block = game->getBlockAt(b->x, b->y, b->z + 1);
		if (block)
		{
			b->setOneFace((int)Faces::Front, false);
			block->setOneFace((int)Faces::Back, false);
		}
		block = game->getBlockAt(b->x, b->y, b->z - 1);
		if (block)
		{
			block->setOneFace((int)Faces::Front, false);
			b->setOneFace((int)Faces::Back, false);
		}
		block = game->getBlockAt(b->x + 1, b->y, b->z);
		if (block)
		{
			block->setOneFace((int)Faces::Left, false);
			b->setOneFace((int)Faces::Right, false);
		}
		block = game->getBlockAt(b->x - 1, b->y, b->z);
		if (block)
		{
			b->setOneFace((int)Faces::Left, false);
			block->setOneFace((int)Faces::Right, false);
		}
	}
	toAdd.clear();

}

void Chunk::draw()
{
	for (int j = 0; j < chunkH; j++)
		for (int i = 0; i < chunkW; i++)
			for (int k = 0; k < chunkT; k++)
				if (blocks[j][i][k])
					blocks[j][i][k]->draw();
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

bool Chunk::isThisChunk(int x, int y, int z)
{
	x -= this->x * chunkW;
	y -= this->y * chunkH;
	z -= this->z * chunkT;
	return x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT;
}

	