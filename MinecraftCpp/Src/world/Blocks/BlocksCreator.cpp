#include "BlocksCreator.h"
#include "CubeBlock.h"
#include "Plant.h"
#include "OakTree.h"
#include "Leaf.h"
#include "Water.h"
#include "Cactus.h"
#include "SpruceTree.h"

Block* createBlock(int i,int x,int y,int z)
{
	if (i <= -1)
		return NULL;
	switch (i)
	{
	case 0:///Grass
		return new CubeBlock(i, x, y, z, 0,3);
	case 1:///Dirt
		return new CubeBlock(i, x, y, z, 2);
	case 2:///Stone
		return new CubeBlock(i, x, y, z, 3);
	case 3:///Snow
		return new CubeBlock(i, x, y, z, 4, 3);
	case 4:///Sand
		return new CubeBlock(i, x, y, z, 7, 1);
	case 5:///Wood 1
		return new CubeBlock(i, x, y, z, 8, 2);
	case 6:
		return new Leaf(i, x, y, z, 10);
	case 7:///Wood 2
		return new CubeBlock(i, x, y, z, 12, 2);
	case 8:
		return new Leaf(i, x, y, z, 14);
	case 9:///Wood 3
		return new CubeBlock(i, x, y, z, 16, 2);
	case 10:
		return new Leaf(i, x, y, z, 18);

	case 11:///WATER
		return new Water(i, x, y, z, 23);
	case 12:///Sapling 1
		return new OakTree(i, x, y, z, 11);
	case 13:///Sapling 2
		return new OakTree(i, x, y, z, 15, 5, 6);
	case 14:///Sapling 3
		return new SpruceTree(i, x, y, z, 19, 9, 8);
	case 15:///Cactus
		return new Cactus(i, x, y, z, 20);
		break;
	case 16://Grass
		return new Plant(i, x, y, z, 24);
	case 17://Yellow Flower
		return new Plant(i, x, y, z, 25);
	case 18://Red Flower
		return new Plant(i, x, y, z, 26);
	case 19://Blue Flower
		return new Plant(i, x, y, z, 27);
	case 20://ICE
		return new CubeBlock(i, x, y, z, 28, 1, true);
	case 21://Berries
		return new Plant(i, x, y, z, 29);
	case 22://Sugger cane
		return new Plant(i, x, y, z, 30);
	case 23:///Plank 1
		return new CubeBlock(i, x, y, z, 31);
	case 24:///Plank 2
		return new CubeBlock(i, x, y, z, 32);
	case 25:///Plank 3
		return new CubeBlock(i, x, y, z, 33);
	case 26:///CoobleStone
		return new CubeBlock(i, x, y, z, 34);
	case 27:///Glass
		return new CubeBlock(i, x, y, z, 35);
	case 28:///SandStone
		return new CubeBlock(i, x, y, z, 36, 3);
	case 29://PathDirt
		return new CubeBlock(i, x, y, z, 39, 3);
	default:
		return NULL;
	}
	return NULL;
}

