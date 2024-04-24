#include "CubeHouse.h"
#include "../../scene/Game.h"
#include "BlocksCreator.h"

CubeHouse::CubeHouse(int ID, int x, int y, int z, int woodID, int plankID, int florID) :Block(ID, x, y, z)
{
	this->woodID = woodID;
	this->plankID = plankID;
	this->florID = florID;
}

void CubeHouse::update(float deltaTime)
{
	Game* g = getCurrentGame();
	if (!g)
		return;

	g->deleteBlock(x, y, z);
	Block* b = createBlock(florID, x, y, z);
	if (!g->addBlock(b))
		delete b;

	int n = 3;
	int h = 3;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			g->deleteBlock(x + i, y, z + j);
			g->deleteBlock(x - i, y, z + j);
			g->deleteBlock(x - i, y, z - j);
			g->deleteBlock(x + i, y, z - j);
			if (!g->addBlock(b = createBlock(florID, x + i, y, z + j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(florID, x - i, y, z + j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(florID, x - i, y, z - j)) && b)
				delete b;
			if (!g->addBlock(b = createBlock(florID, x + i, y, z - j)) && b)
				delete b;
		}
	for (int i = 0; i < h; i++)
	{
		g->deleteBlock(x + n - 1, y + 1 + i, z + n - 1);
		g->deleteBlock(x - n + 1, y + 1 + i, z + n - 1);
		g->deleteBlock(x - n + 1, y + 1 + i, z - n + 1);
		g->deleteBlock(x + n - 1, y + 1 + i, z - n + 1);

		if (!g->addBlock(b = createBlock(woodID, x + n - 1, y + 1 + i, z + n - 1)) && b)
			delete b;
		if (!g->addBlock(b = createBlock(woodID, x - n + 1, y + 1 + i, z + n - 1)) && b)
			delete b;
		if (!g->addBlock(b = createBlock(woodID, x - n + 1, y + 1 + i, z - n + 1)) && b)
			delete b;
		if (!g->addBlock(b = createBlock(woodID, x + n - 1, y + 1 + i, z - n + 1)) && b)
			delete b;
		for (int j = -n + 1; j < n; j++)
		{
			g->deleteBlock(x + j, y + 1 + i, z + n - 1);
			if (!g->addBlock(b = createBlock(plankID, x + j, y + 1 + i, z + n - 1)) && b)
				delete b;
			g->deleteBlock(x + j, y + 1 + i, z - n + 1);
			if (!g->addBlock(b = createBlock(plankID, x + j, y + 1 + i, z - n + 1)) && b)
				delete b;

			g->deleteBlock(x + n - 1, y + 1 + i, z + j);
			if (!g->addBlock(b = createBlock(plankID, x + n - 1, y + 1 + i, z + j)) && b)
				delete b;
			g->deleteBlock(x - n + 1, y + 1 + i, z + j);
			if (!g->addBlock(b = createBlock(plankID, x - n + 1, y + 1 + i, z + j)) && b)
				delete b;
		}
	}
	y += h + 1;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			g->deleteBlock(x + i, y, z + j);
			g->deleteBlock(x - i, y, z + j);
			g->deleteBlock(x - i, y, z - j);
			g->deleteBlock(x + i, y, z - j);
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


}

void CubeHouse::setFacing(int faces)
{

}

