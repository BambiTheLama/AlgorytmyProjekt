#include "CubeHouse.h"
#include "../../scene/Game.h"
#include "BlocksCreator.h"

CubeHouse::CubeHouse(int ID, int x, int y, int z, int woodID, int plankID, int floorID) :Block(ID, x, y, z)
{
	this->woodID = woodID;
	this->plankID = plankID;
	this->floorID = floorID;
}

void CubeHouse::update(float deltaTime)
{
	Game* g = getCurrentGame();
	if (!g)
		return;

	g->deleteBlock(x, y, z);
	Block* b = createBlock(floorID, x, y, z);
	if (!g->addBlock(b))
		delete b;

	int n = 3;
	int h = 3;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k <= h; k++)
			{
				g->deleteBlock(x + i, y + k, z + j);
				g->deleteBlock(x - i, y + k, z + j);
				g->deleteBlock(x - i, y + k, z - j);
				g->deleteBlock(x + i, y + k, z - j);
			}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			if (!g->addBlock(b = createBlock(floorID, x + i, y, z + j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(floorID, x - i, y, z + j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(floorID, x - i, y, z - j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(floorID, x + i, y, z - j)) && b)
				delete b;
		}
	for (int i = 0; i < h; i++)
	{
		if (!g->addBlock(b = createBlock(woodID, x + n - 1, y + 1 + i, z + n - 1)) && b)
			delete b;
		if (!g->addBlock(b = createBlock(woodID, x - n + 1, y + 1 + i, z + n - 1)) && b)
			delete b;
		if (!g->addBlock(b = createBlock(woodID, x - n + 1, y + 1 + i, z - n + 1)) && b)
			delete b;
		if (!g->addBlock(b = createBlock(woodID, x + n - 1, y + 1 + i, z - n + 1)) && b)
			delete b;
		for (int j = -n + 2; j < n-1; j++)
		{
			if (!g->addBlock(b = createBlock(plankID, x + j, y + 1 + i, z + n - 1)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(plankID, x + j, y + 1 + i, z - n + 1)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(plankID, x + n - 1, y + 1 + i, z + j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(plankID, x - n + 1, y + 1 + i, z + j)) && b)
				delete b;
		}
	}
	y += h + 1;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{

			if (!g->addBlock(b = createBlock(plankID, x + i, y, z + j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(plankID, x - i, y, z + j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(plankID, x - i, y, z - j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(plankID, x + i, y, z - j)) && b)
				delete b;
		}

	g->deleteBlock(x + n - 1, y, z + n - 1);
	g->deleteBlock(x - n + 1, y, z + n - 1);
	g->deleteBlock(x - n + 1, y, z - n + 1);
	g->deleteBlock(x + n - 1, y, z - n + 1);

	if (!g->addBlock(b = createBlock(woodID, x + n - 1, y, z + n - 1)) && b)
		delete b;
	if (!g->addBlock(b = createBlock(woodID, x - n + 1, y, z + n - 1)) && b)
		delete b;
	if (!g->addBlock(b = createBlock(woodID, x - n + 1, y, z - n + 1)) && b)
		delete b;
	if (!g->addBlock(b = createBlock(woodID, x + n - 1, y, z - n + 1)) && b)
		delete b;
	y -= h + 1;

	g->deleteBlock(x, y + 2, z - n + 1);
	g->deleteBlock(x, y + 2, z + n - 1);
	g->deleteBlock(x - n + 1, y + 2, z);
	g->deleteBlock(x + n - 1, y + 2, z);
	if (!g->addBlock(b = createBlock(27, x, y + 2, z - n + 1)) && b)
		delete b;
	if (!g->addBlock(b = createBlock(27, x, y + 2, z + n - 1)) && b)
		delete b;
	if (!g->addBlock(b = createBlock(27, x - n + 1, y + 2, z)) && b)
		delete b;
	if (!g->addBlock(b = createBlock(27, x + n - 1, y + 2, z)) && b)
		delete b;

	g->deleteBlock(x, y + 2, z - n + 1);
	g->deleteBlock(x, y + 1, z - n + 1);
	


}