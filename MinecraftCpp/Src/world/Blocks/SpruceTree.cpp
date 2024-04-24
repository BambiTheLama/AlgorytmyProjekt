#include "SpruceTree.h"
#include "../../scene/Game.h"
#include "BlocksCreator.h"

SpruceTree::SpruceTree(int ID, int x, int y, int z, int textureID, int woodID, int leafID):Plant(ID,x,y,z,textureID)
{
	this->woodID = woodID;
	this->leafID = leafID;
}

void SpruceTree::update(float deltaTime)
{
	Game* g = getCurrentGame();
	if (!g)
		return;
	g->deleteBlock(x, y, z);
	int n = rand() % 6 + 6;
	for (int i = 0; i < n; i++)
	{
		Block* b = createBlock(woodID, x, y + i, z);
		if (!b)
			continue;
		if (!g->addBlock(b))
			delete b;
	}

	for (int x = 0; x < 5; x++)
		for (int z = 0; z < 5; z++)
		{
			Block* b = createBlock(leafID, this->x + x - 2, this->y + n - 6, this->z + z - 2);

			if (b && !g->addBlock(b))
				delete b;
			b = createBlock(leafID, this->x + x - 2, this->y + n - 2, this->z + z - 2);
			if (b && !g->addBlock(b))
				delete b;
		}
	for (int x = 0; x < 3; x++)
		for (int z = 0; z < 3; z++)
		{
			Block* b = createBlock(leafID, this->x + x - 1, this->y + n-5, this->z + z - 1);

			if (b && !g->addBlock(b))
				delete b;
			b = createBlock(leafID, this->x + x - 1, this->y + n-3, this->z + z - 1);
			if (b && !g->addBlock(b))
				delete b;
			b = createBlock(leafID, this->x + x - 1, this->y + n-1, this->z + z - 1);
			if (b && !g->addBlock(b))
				delete b;
		}
	Block* b = createBlock(leafID, this->x, this->y + n, this->z);
	if (b && !g->addBlock(b))
		delete b;
}