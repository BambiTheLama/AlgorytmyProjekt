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
	int n = 3;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			g->deleteBlock(x + i, y, z + j);
			g->deleteBlock(x - i, y, z + j);
			g->deleteBlock(x - i, y, z - j);
			g->deleteBlock(x + i, y, z - j);
			g->addBlock(createBlock(florID, x + i, y, z + j));
			g->addBlock(createBlock(florID, x - i, y, z + j));
			g->addBlock(createBlock(florID, x - i, y, z - j));
			g->addBlock(createBlock(florID, x + i, y, z - j));
		}

}

void CubeHouse::setFacing(int faces)
{

}