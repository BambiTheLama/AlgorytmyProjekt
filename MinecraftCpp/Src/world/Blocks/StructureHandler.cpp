#include "StructureHandler.h"
#include "../../scene/Game.h"
#include "BlocksCreator.h"

StructureHandler::StructureHandler(int ID, int x, int y, int z, int w, int h, int t) :Block(ID, x, y, z)
{
	this->t = t;
	this->h = h;
	this->w = w;
	structure = new int[t * h * w];
	for (int i = 0; i < t * h * w; i++)
		structure[i] = -1;
}

StructureHandler::~StructureHandler()
{
	delete structure;
}

void StructureHandler::update(float deltaTime)
{
	Game* g = getCurrentGame();
	if (!g)
		return;
	g->deleteBlock(x, y, z);
	int startX = x - w / 2;
	int startZ = z - t / 2;
	Block* b;
	if (h == -1)
	{
		for (int i = 0; i < w; i++)
			for (int j = 0; j < t; j++)
			{
				int blockId = structure[i + j * w];
				if (blockId < 0)
					continue;
				int h = y - 5;
				
				for (; h < chunkH; h++)
				{
					if (!g->isBlockAt(startX + i, h, startZ + j))
					{
						h--;
						break;
					}
				}
				g->deleteBlock(startX + i, h, startZ + j);
				b = createBlock(blockId, startX + i, h, startZ + j);
				if (!g->addBlock(b) && b)
					delete b;
			}
		return;
	}
	for (int i = 0; i < w; i++)
		for (int j = 0; j < t; j++)
			for (int k = 0; k < h; k++)
			{
				int blockId = structure[i + j * w + k * w * t];
				if (blockId < 0)
					continue;
				g->deleteBlock(startX + i, y + k, startZ + j);
				b = createBlock(blockId, startX + i, y + k, startZ + j);
				if (!g->addBlock(b) && b)
					delete b;
			}
}

void StructureHandler::rotate()
{
	int *structureTmp = new int[t * h * w];
	for (int i = 0; i < w; i++)
		for (int j = 0; j < t; j++)
			for (int k = 0; k < h; k++)
			{
				structureTmp[t - 1 - j + i * t + k * w * t] = structure[i + j * w + k * w * t];
			}
	int tmp = w;
	w = t;
	t = tmp;
	delete structure;
	structure = structureTmp;
	timesToRotate++;
}

void StructureHandler::setBlock(int x, int y,int z, int ID)
{
	if (x < 0 || x >= w || y < 0 || y >= h || z < 0 || z >= t)
		return;
	structure[x + z * w + y * w * t] = ID;
}

void StructureHandler::removeBlock(int x, int y,int z)
{
	if (x < 0 || x >= w || y < 0 || y >= h || z < 0 || z >= t)
		return;
	structure[x + z * w + y * w * t] = -1;
}

void StructureHandler::swapBlocks(int oldID, int newID)
{
	for (int i = 0; i < h * w * t; i++)
		if (structure[i] == oldID)
			structure[i] = newID;
}

void StructureHandler::setVariant(int variant)
{
	if (this->variant == 0)
	{
	}
	else if (this->variant == 1)
	{
		swapBlocks(28, 26);
	}
	else if (this->variant == 2)
	{
		swapBlocks(9, 5);
		swapBlocks(25, 23);
	}

	if (variant == 0)
	{
	}
	else if (variant == 1)
	{
		swapBlocks(26, 28);
	}
	else if (variant == 2)
	{
		swapBlocks(5, 9);
		swapBlocks(23, 25);
	}
	this->variant = variant;
}