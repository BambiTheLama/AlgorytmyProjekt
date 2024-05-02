#include "StructureHalder.h"
#include "../../scene/Game.h"
#include "BlocksCreator.h"

StructureHalder::StructureHalder(int ID, int x, int y, int z, int w, int h, int t) :Block(ID, x, y, z)
{
	this->t = t;
	this->h = h;
	this->w = w;
	structure = new int[t * h * w];
	for (int i = 0; i < t * h * w; i++)
		structure[i] = -1;
}

StructureHalder::~StructureHalder()
{
	delete structure;
}

void StructureHalder::update(float deltaTime)
{
	Game* g = getCurrentGame();
	if (!g)
		return;
	g->deleteBlock(x, y, z);
	int startX = x - w / 2;
	int startZ = z - t / 2;
	Block* b;
	if (h < 0)
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

void StructureHalder::rotate()
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

void StructureHalder::setBlock(int x, int y,int z, int ID)
{
	if (x < 0 || x >= w || y < 0 || y >= h || z < 0 || z >= t)
		return;
	structure[x + z * w + y * w * t] = ID;
}

void StructureHalder::removeBlock(int x, int y,int z)
{
	if (x < 0 || x >= w || y < 0 || y >= h || z < 0 || z >= t)
		return;
	structure[x + z * w + y * w * t] = -1;
}

void StructureHalder::swapBlocks(int oldID, int newID)
{
	for (int i = 0; i < h * w * t; i++)
		if (structure[i] == oldID)
			structure[i] = newID;
}