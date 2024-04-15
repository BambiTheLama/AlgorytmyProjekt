#include "OakTree.h"
#include "../../scene/Game.h"
#include "BlocksCreator.h"
OakTree::OakTree(int ID, int x, int y, int z, int textureID, int woodID, int leafID) :Plant(ID, x, y, z, textureID)
{
	this->woodID = woodID;
	this->leafID = leafID;
}

void OakTree::update(float deltaTime)
{
	Game* g = getCurrentGame();
	if (!g)
		return;
	g->deleteBlock(x, y, z);
	//return;
	for (int i = 0; i < 5; i++)
	{
		Block* b = createBlock(woodID, x, y + i, z);
		if (!b)
			continue;
		if (!g->addBlock(b))
			delete b;
	}
	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 5; x++)
			for (int z = 0; z < 5; z++)
			{
				Block* b = createBlock(leafID, this->x + x - 2, this->y + y + 3, this->z + z - 2);
				if (!b)
					continue;
				if (!g->addBlock(b))
					delete b;
			}
		for (int x = 0; x < 3; x++)
			for (int z = 0; z < 3; z++)
			{
				Block* b = createBlock(leafID, this->x + x - 1, this->y + y + 5, this->z + z - 1);
				if (!b)
					continue;
				if (!g->addBlock(b))
					delete b;
			}
	}


}