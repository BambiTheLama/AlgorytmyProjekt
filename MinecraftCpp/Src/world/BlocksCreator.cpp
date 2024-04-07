#include "BlocksCreator.h"
#include "CubeBlock.h"
#include "Plant.h"
#include "OakTree.h"
#include "Leaf.h"

Block* createBlock(int i,int x,int y,int z)
{
	if (i <= -1)
		return NULL;
	switch (i)
	{
	case 0:///Grass
		return new CubeBlock(i, x, y, z, 0, 0, 3);
	case 1:///Dirt
		return new CubeBlock(i, x, y, z, 2, 0, 1);
	case 2:///Stone
		return new CubeBlock(i, x, y, z, 3, 0, 1);
	case 3:///Snow
		return new CubeBlock(i, x, y, z, 0, 1, 3);
	case 4:///Sand
		return new CubeBlock(i, x, y, z, 3, 1, 1);
	case 5:///Wood 1
		return new CubeBlock(i, x, y, z, 0, 2, 2);
	case 6:
		return new Leaf(i, x, y, z, 2, 2);
	case 7:///Wood 2
		return new CubeBlock(i, x, y, z, 0, 3, 2);
	case 8:
		return new Leaf(i, x, y, z, 2, 3);
	case 9:///Wood 3
		return new CubeBlock(i, x, y, z, 0, 4, 2);
	case 10:
		return new Leaf(i, x, y, z, 2, 4);

	case 11:///WATER
		return new CubeBlock(i, x, y, z, 4, 0, 1, true);
	case 12:///Sapling 1
		return new OakTree(i, x, y, z, 3, 2);
	case 13:///Sapling 2
		return new OakTree(i, x, y, z, 3, 3,5,6);
	case 14:///Sapling 3
		return new Plant(i, x, y, z, 3, 4);
	case 15:///Sapling 3
		//return new Cactus(i, x, y, z, 3, 4);
		break;

	default:
		return NULL;
	}
	return NULL;
}

