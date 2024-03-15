#include "BlocksCreator.h"
#include "CubeBlock.h"

Block* createBlock(int i)
{
	switch (i)
	{
	case 0:
		return new CubeBlock(0, 0, 0, 0, 0, 3);
	case 1:
		return new CubeBlock(0, 0, 0, 2, 0, 1);
	case 2:
		return new CubeBlock(0, 0, 0, 3, 0, 1);
	case 3:
		return new CubeBlock(0, 0, 0, 0, 1, 3);
	case 4:
		return new CubeBlock(0, 0, 0, 3, 1, 1);
	case 5:
		return new CubeBlock(0, 0, 0, 0, 2, 2);
	case 6:
		return new CubeBlock(0, 0, 0, 2, 2, 2);
	case 7:
		return new CubeBlock(0, 0, 0, 0, 3, 2);
	case 8:
		return new CubeBlock(0, 0, 0, 2, 3, 2);
	case 9:
		return new CubeBlock(0, 0, 0, 0, 4, 2);
	case 10:
		return new CubeBlock(0, 0, 0, 2, 4, 2);
	default:
		return NULL;
	}
	return NULL;
}

