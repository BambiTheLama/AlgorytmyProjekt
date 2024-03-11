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
				blocks[j][i][k] = new CubeBlock(i + x * chunkW, j + y * chunkH, k + z * chunkT, "Res/1.jpg");
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



void Chunk::draw(glm::vec3 playerPos)
{
	int y = playerPos.y - this->y * chunkH;
	int x = playerPos.x - this->x * chunkW;
	int z = playerPos.z - this->z * chunkT;
	if (y <= 0)
	{
		for (int i = chunkH - 1; i >= 0; i--)
			drawLineY(x, i, z);
	}
	else if (y >= chunkH)
	{
		for (int i = 0; i < chunkH; i++)
			drawLineY(x, i, z);
	}
	else
	{
		for (int i = chunkH - 1; i >= y; i--)
			drawLineY(x, i, z);
		for (int i = 0; i < y; i++)
			drawLineY(x, i, z);
	}
}

void Chunk::drawLineY(int x, int y, int z)
{
	if (x <= 0)
	{
		for (int i = 0; i < chunkW; i++)
			drawLineX(i, y, z);
	}
	else if (x >= chunkW)
	{

		for (int i = chunkW - 1; i >= 0; i--)
			drawLineX(i, y, z);
	}
	else
	{
		for (int i = chunkW - 1; i >= x; i--)
			drawLineX(i, y, z);
		for (int i = 0; i < x; i++)
			drawLineX(i, y, z);
	}
}

void Chunk::drawLineX(int x, int y, int z)
{
	if (z <= 0)
	{
		for (int i = 0; i < chunkT; i++)
			drawLineZ(x, y, i);
	}
	else if (z >= chunkT)
	{
		for (int i = chunkT - 1; i >= 0; i--)
			drawLineZ(x, y, i);
	}
	else
	{
		for (int i = chunkT - 1; i >= z; i--)
			drawLineZ(x, y, i);
		for (int i = 0; i < z; i++)
			drawLineZ(x, y, i);
	}
}

void Chunk::drawLineZ(int x, int y, int z)
{
	if (blocks[y][x][z])
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

	