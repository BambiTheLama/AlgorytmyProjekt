#include "BlocksCreator.h"
#include "CubeBlock.h"
#include "Plant.h"
#include "OakTree.h"
#include "Leaf.h"
#include "Water.h"
#include "Cactus.h"
#include "SpruceTree.h"
#include "StructureHalder.h"

Block* createBlock(int i, int x, int y, int z)
{
	if (i <= -1)
		return createStructure(-i, x, y, z);
	switch (i)
	{
	case 0:///Grass
		return new CubeBlock(i, x, y, z, 0, 3);
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
		return new CubeBlock(i, x, y, z, 35, 1, true);
	case 28:///SandStone
		return new CubeBlock(i, x, y, z, 36, 3);
	case 29://PathDirt
		return new Water(i, x, y, z, 39,3);
	default:
		return NULL;
	}
	return NULL;
}

StructureHalder* createStructure(int i, int x, int y, int z)
{
	StructureHalder* structure = NULL;
	const int w = StructureTileSize;
	const int t = StructureTileSize;
	switch (i)
	{
	case 1:{

		structure = new StructureHalder(-i, x, y, z, w, 5, t);
		for (int x = w / 2 - 2; x < w / 2 + 3; x++)
			for (int z = t / 2 - 2; z < t / 2 + 3; z++)
				structure->setBlock(x, 0, z, 26);
		for (int x = w / 2 - 1; x < w / 2 + 2; x++)
			for (int y = 1; y < 5; y++)
			{
				structure->setBlock(x, y, w / 2 - 2, 23);
				structure->setBlock(x, y, w / 2 + 2, 23);
				structure->setBlock(w / 2 - 2, y, x, 23);
				structure->setBlock(w / 2 + 2, y, x, 23);
			}
		for (int x = w / 2 - 1; x < w / 2 + 2; x++)
			for (int z = t / 2 - 1; z < t / 2 + 2; z++)
				structure->setBlock(x, 4, z, 23);
		for (int y = 1; y < 5; y++)
		{
			structure->setBlock(w / 2 - 2, y, t / 2 - 2, 5);
			structure->setBlock(w / 2 - 2, y, t / 2 + 2, 5);
			structure->setBlock(w / 2 + 2, y, t / 2 - 2, 5);
			structure->setBlock(w / 2 + 2, y, t / 2 + 2, 5);
		}
		structure->setBlock(w / 2 - 2, 2, t / 2, 27);
		structure->setBlock(w / 2 + 2, 2, t / 2, 27);
		structure->setBlock(w / 2, 2, t / 2 - 2, 27);

		structure->setBlock(w / 2, 2, t / 2 + 2, 1000000000);
		structure->setBlock(w / 2, 1, t / 2 + 2, 1000000000);
		for (int z = t / 2 + 3; z < t; z++)
			for (int x = w / 2 - 1; x < w / 2 + 2; x++)
				structure->setBlock(x, 0, z, 29);
		break;
	}
	case 2:{
		structure = new StructureHalder(-i, x, y, z, w, 1, t);

		for (int z = 0; z < t; z++)
			for (int x = w / 2 - 1; x < w / 2 + 2; x++)
				structure->setBlock(x, 0, z, 29);
		break;
	}
	case 3: {
		structure = new StructureHalder(-i, x, y, z, w, 1, t);
		for (int z = 0; z < t; z++)
			for (int x = w / 2 - 1; x < w / 2 + 2; x++)
				structure->setBlock(x, 0, z, 29);
		for (int z = t / 2 - 1; z < t / 2 + 2; z++)
			for (int x = w / 2 + 1; x < w; x++)
				structure->setBlock(x, 0, z, 29);
		break;
	}
	case 4: {
		structure = new StructureHalder(-i, x, y, z, w, 1, t);

		for (int z = 0; z < w / 2 + 1; z++)
			for (int x = w / 2 - 1; x < w / 2 + 2; x++)
				structure->setBlock(x, 0, z, 29);
		for (int z = t / 2 - 1; z < t / 2 + 2; z++)
			for (int x = w / 2 - 1; x < w; x++)
				structure->setBlock(x, 0, z, 29);
		break;
	}
	case 5: {
		structure = new StructureHalder(-i, x, y, z, w, 1, t);

		for (int z = 0; z < t; z++)
			for (int x = w / 2 - 1; x < w / 2 + 2; x++)
			{
				structure->setBlock(x, 0, z, 29);
				structure->setBlock(z, 0, x, 29);
			}

		break;
	}
	case 6:{
		structure = new StructureHalder(-i, x, y, z, w, 8, t);
		for (int z = t / 2 + 1; z < t; z++)
			for (int x = w / 2 - 1; x < w / 2 + 2; x++)
				structure->setBlock(x, 0, z, 29);

		for (int z = t / 2 - 3; z < t / 2 + 4; z++)
		{
			for (int x = 1; x < w - 2; x++)
				structure->setBlock(x, 0, z, 26);
			for (int y = 1; y < 4; y++)
			{
				structure->setBlock(1, y, z, 23);
				structure->setBlock(w-2, y, z, 23);
			}

		}
		for (int y = 1; y < 4; y++)
			for (int x = 1; x < w-2; x++)
			{
				structure->setBlock(x, y, t/2-3, 23);
				structure->setBlock(x, y, t/2+3, 23);
			}
		structure->setBlock(w/2, 1, t / 2+3, 100000);
		structure->setBlock(w/2, 2, t / 2+3, 100000);

		for (int x = 2; x < w - 2; x++)
			structure->setBlock(x, 2, t / 2 - 3, 27);
		for (int x = 2; x < w / 2 - 1; x++)
		{
			structure->setBlock(x, 2, t / 2 + 3, 27);
			structure->setBlock(x + w / 2, 2, t / 2 + 3, 27);

		}
		for (int y = 0; y < 4; y++)
			for (int z = t / 2 - 4 + y; z < t / 2 + 5 - y; z++)
				for (int x = y; x < w - y ; x++)
					structure->setBlock(x, 4 + y, z, 26);

		break;
	}
	case 7: {
		structure = new StructureHalder(-i, x, y, z, w, 8, t);
		for (int z = t / 2 + 1; z < t; z++)
			for (int x = w / 2 - 1; x < w / 2 + 2; x++)
				structure->setBlock(x, 0, z, 29);

		for (int z = 1; z < t -2; z++)
		{
			for (int x = 3; x < w - 3; x++)
				structure->setBlock(x, 0, z, 26);
			for (int y = 1; y < 5; y++)
			{
				structure->setBlock(3, y, z, 26);
				structure->setBlock(w - 4, y, z, 26);
			}

		}
		for (int y = 1; y < 5; y++)
			for (int x = 3; x < w - 4; x++)
			{
				structure->setBlock(x, y, 1, 26);
				structure->setBlock(x, y, t-3, 26);
			}

		for (int z = 1; z < t - 2; z++)
			for (int x = 3; x < w - 3; x++)
				structure->setBlock(x, 5, z, 26);


		for (int z = 3; z < t - 4; z++)
			for (int y = 2; y < 4; y++)
			{
				structure->setBlock(3, y, z, 27);
				structure->setBlock(w - 4, y, z, 27);
			}
		for (int y = 2; y < 4; y++)
			for (int x = 5; x < w - 5; x++)
			{
				structure->setBlock(x, y, 1, 27);
			}

		structure->setBlock(w / 2, 1, t - 3, 100000);
		structure->setBlock(w / 2, 2, t - 3, 100000);
		break;
	}
	case 8: {
		structure = new StructureHalder(-i, x, y, z, w, 10, t);

		for (int z = 0; z < t; z++)
			for (int x = w / 2 - 1; x < w / 2 + 2; x++)
			{
				structure->setBlock(x, 0, z, 29);
				structure->setBlock(z, 0, x, 29);
			}
		for (int z = 2; z < t-2; z++)
			for (int x = 2; x < w-2; x++)
				structure->setBlock(x, 0, z, 29);

		for (int y = 0; y < 10; y++)
		{
			structure->setBlock(w / 2, y, t / 2, 26);
		}
		for (int i = 1; i < 3; i++)
		{
			structure->setBlock(w / 2 + i, 7, t / 2, 26);
			structure->setBlock(w / 2 - i, 7, t / 2, 26);

		}
		for (int y = 0; y < 4; y++)
			for (int x = w / 2 - 3 + y; x < w / 2 + 4 - y; x++)
				for (int z = t / 2 - 3 + y; z < t / 2 + 4 - y; z++)
					structure->setBlock(x, y, z, 26);
				



		break;
	}
	case 9: {
		structure = new StructureHalder(-i, x, y, z, w, 10, t);
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < w / 2; x++)
			{
				for (int i = 0; i < 3; i++)
				{
					structure->setBlock(x, y, t / 2 - 1 + i, 26);
					structure->setBlock(w / 2 - 1 + i, y, x, 26);
				}

			}
		}
		for (int x = 1; x < w / 2; x+=2)
		{
			structure->setBlock(x, 5, t / 2 - 1, 26);
			structure->setBlock(x, 5, t / 2 + 1, 26);
			structure->setBlock(t / 2 - 1, 5, x, 26);
			structure->setBlock(t / 2 + 1, 5, x, 26);
		}
		for (int y = 0; y < 9; y++)
			for (int x = w / 2 - 3; x < w / 2 + 4; x++)
				for (int z = t / 2 - 3; z < t / 2 + 4; z++)
					structure->setBlock(x, y, z, 26);

		for (int y = 1; y < 8; y++)
			for (int x = w / 2 - 2; x < w / 2 + 3; x++)
				for (int z = t / 2 - 2; z < t / 2 + 3; z++)
					structure->setBlock(x, y, z, 1000000);

		for (int x = w / 2 - 3; x < w / 2 + 4; x += 2)
		{
			structure->setBlock(x, 9, t / 2 - 3, 26);
			structure->setBlock(x, 9, t / 2 + 3, 26);
			structure->setBlock(w / 2 - 3, 9, x, 26);
			structure->setBlock(w / 2 + 3, 9, x, 26);
		}
		break;
	}
	case 10: {
		structure = new StructureHalder(-i, x, y, z, w, 6, t);
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < w; x++)
			{
				for (int i = 0; i < 3; i++)
				{
					structure->setBlock(x, y, t / 2 - 1 + i, 26);
				}

			}
		}
		for (int x = 0; x < w; x += 2)
		{
			structure->setBlock(x, 5, t / 2 - 1, 26);
			structure->setBlock(x, 5, t / 2 + 1, 26);
		}

		break;
	}
	case 11: {
		structure = new StructureHalder(-i, x, y, z, w, 10, t);
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < w; x++)
			{
				for (int i = 0; i < 3; i++)
				{
					structure->setBlock(x, y, t / 2 - 1 + i, 26);
				}

			}
		}
		for (int x = w / 2 - 3; x < w / 2 + 4; x += 2)
		{
			structure->setBlock(x, 9, t / 2 - 3, 26);
			structure->setBlock(x, 9, t / 2 + 3, 26);
			structure->setBlock(w / 2 - 3, 9, x, 26);
			structure->setBlock(w / 2 + 3, 9, x, 26);
		}
		for (int x = 0; x < w; x += 2)
		{
			structure->setBlock(x, 5, t / 2 - 1, 26);
			structure->setBlock(x, 5, t / 2 + 1, 26);
		}

		for (int y = 0; y < 9; y++)
			for (int x = w / 2 - 3; x < w / 2 + 4; x++)
				for (int z = t / 2 - 3; z < t / 2 + 4; z++)
					structure->setBlock(x, y, z, 26);

		for (int y = 1; y < 8; y++)
			for (int x = w / 2 - 2; x < w / 2 + 3; x++)
				for (int z = t / 2 - 2; z < t / 2 + 3; z++)
					structure->setBlock(x, y, z, 1000000);

		for (int z = 0; z < t; z++)
		{
			for (int i = 0; i < 3; i++)
			{
				structure->setBlock(w / 2 - 1 + i, 0, z, 29);
				for (int y = 1; y < 4; y++)
					structure->setBlock(w / 2 - 1 + i, y, z, 1000000);

			}

		}

		break;
	}

	}
	return structure;
}