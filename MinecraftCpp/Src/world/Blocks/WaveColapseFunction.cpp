#include "WaveColapseFunction.h"
#include <vector>
#include <algorithm>
Tile::Tile(int ID, int up, int down, int left, int right, int rotate, bool building)
{
	this->ID = ID;
	this->up = up;
	this->down = down;
	this->left = left;
	this->right = right;
	this->rotate = rotate % 4;
	this->building = building;
	for (int i = 0; i < this->rotate; i++)
		rotateTile();
}

void Tile::rotateTile()
{
	int tmp = up;
	up = left;
	left = down;
	down = right;
	right = tmp;
}

bool Tile::canConnect(Dir dir, Tile tile)
{

	switch (dir)
	{
	case Dir::Up:
		if(!building || !tile.building)
			return tile.down == up;
		return up == 0 && tile.down == up;
	case Dir::Down:
		if (!building || !tile.building)
			return tile.up == down;
		return down == 0 && tile.up == down;
	case Dir::Left:
		if (!building || !tile.building)
			return tile.right == left;
		return left == 0 && tile.right == left;
	case Dir::Right:
		if (!building || !tile.building)
			return tile.left == right;
		return right == 0 && tile.left == right;
	}
	return false;	
}

static std::vector<Tile> allPossibleTiles{

	Tile(),

	Tile(1,0,1,0,0,0,true),
	Tile(1,0,1,0,0,1,true),
	Tile(1,0,1,0,0,2,true),
	Tile(1,0,1,0,0,3,true),

	Tile(2,1,1),
	Tile(2,1,1,0,0,1),

	Tile(3,1,1,0,1,0),
	Tile(3,1,1,0,1,1),
	Tile(3,1,1,0,1,2),
	Tile(3,1,1,0,1,3),
	//10

	//11
	Tile(4,1,0,0,1,0),
	Tile(4,1,0,0,1,1),
	Tile(4,1,0,0,1,2),
	Tile(4,1,0,0,1,3),

	Tile(5,1,1,1,1,0),

	Tile(6,0,1,0,0,0,true),
	Tile(6,0,1,0,0,1,true),
	Tile(6,0,1,0,0,2,true),
	Tile(6,0,1,0,0,3,true),
	//19

	//20
	Tile(7,0,1,0,0,0,true),
	Tile(7,0,1,0,0,1,true),
	Tile(7,0,1,0,0,2,true),
	Tile(7,0,1,0,0,3,true),

	Tile(8,1,1,1,1,0,true),
	Tile(8,1,1,1,1,1,true),
};

static std::vector<Tile> wall{
	Tile(9,2,0,0,2,0,true),
	Tile(9,2,0,0,2,1,true),
	Tile(9,2,0,0,2,2,true),
	Tile(9,2,0,0,2,3,true),

	Tile(10,0,0,2,2,0,false),
	Tile(10,0,0,2,2,1,false),

	Tile(11,1,1,2,2,0,true),
	Tile(11,1,1,2,2,1,true),
};

struct PossibleTiles
{
	int x, y;
	std::vector<int> tiles;

	int randTile()
	{
		return tiles[rand() % tiles.size()];
	}
};

static bool compareTiles(PossibleTiles* t1, PossibleTiles* t2)
{
	return t1->tiles.size() < t2->tiles.size();
}

static Tile* getTileAt(Tile** tiles, int r, int x, int y)
{
	if (x < 0 || x >= r || y < 0 || y >= r)
		return NULL;
	return tiles[x + y * r];
}

static void genPossibleTiles(Tile** tiles, int r, PossibleTiles* t)
{
	std::vector<int> possibleID;
	Tile* tileUp = getTileAt(tiles, r, t->x, t->y - 1);
	Tile* tileDown = getTileAt(tiles, r, t->x, t->y + 1);
	Tile* tileLeft = getTileAt(tiles, r, t->x - 1, t->y);
	Tile* tileRight = getTileAt(tiles, r, t->x + 1, t->y);
	for (int i = 0; i < allPossibleTiles.size(); i++)
	{
		possibleID.push_back(i);
	}
	if (tileUp)
	{
		std::vector<int> possibleIDTmp;
		for(auto p:possibleID)
		{
			if (tileUp->canConnect(Dir::Down, allPossibleTiles[p]))
				possibleIDTmp.push_back(p);
		}
		possibleID = possibleIDTmp;
	}
	if (tileDown)
	{
		std::vector<int> possibleIDTmp;
		for (auto p : possibleID)
		{
			if (tileDown->canConnect(Dir::Up, allPossibleTiles[p]))
				possibleIDTmp.push_back(p);
		}
		possibleID = possibleIDTmp;
	}
	if (tileLeft)
	{
		std::vector<int> possibleIDTmp;
		for (auto p : possibleID)
		{
			if (tileLeft->canConnect(Dir::Right, allPossibleTiles[p]))
				possibleIDTmp.push_back(p);
		}
		possibleID = possibleIDTmp;
	}
	if (tileRight)
	{
		std::vector<int> possibleIDTmp;
		for (auto p : possibleID)
		{
			if (tileRight->canConnect(Dir::Left, allPossibleTiles[p]))
				possibleIDTmp.push_back(p);
		}
		possibleID = possibleIDTmp;
	}
	if (possibleID.size() <= 0)
		possibleID.push_back(0);
	t->tiles = possibleID;
}

static bool hasPossibleTileAt(int x, int y, std::vector<PossibleTiles*> possibleTiles)
{
	for (auto p : possibleTiles)
		if (p->x == x && p->y == y)
			return true;
	return false;
}

static PossibleTiles* getPossibleTileAt(int x,int y, std::vector<PossibleTiles*> possibleTiles)
{
	for (auto p : possibleTiles)
		if (p->x == x && p->y == y)
			return p;
	return NULL;
}

static void genTile(Tile** tiles, int &r,int x,int y, std::vector<PossibleTiles*>& possibleTiles)
{
	if (x < 0 || x >= r || y < 0 || y >= r)
		return;

	if (!getTileAt(tiles, r, x, y))
	{
		if (hasPossibleTileAt(x, y, possibleTiles))
		{
			PossibleTiles* t = getPossibleTileAt(x, y, possibleTiles);
			if(t)
				genPossibleTiles(tiles, r, t);
		}
		else
		{
			PossibleTiles* t = new PossibleTiles();
			t->x = x;
			t->y = y;
			genPossibleTiles(tiles, r, t);
			possibleTiles.push_back(t);
		}
	}
}

Tile** generateVillage(int r)
{
	Tile** tilesToRet = new Tile*[r * r];
	for (int i = 0; i < r * r; i++)
		tilesToRet[i] = NULL;
	if (r > 5)
	{
		tilesToRet[0] = new Tile(wall[2]);
		tilesToRet[r * r - 1] = new Tile(wall[0]);
		tilesToRet[r - 1] = new Tile(wall[3]);
		tilesToRet[(r - 1)*r] = new Tile(wall[1]);
		for (int i = 1; i < r - 1; i++)
		{
			tilesToRet[(r - 1) * r+i] = new Tile(wall[4]);
			tilesToRet[i] = new Tile(wall[4]);
			tilesToRet[(i) * r] = new Tile(wall[5]);
			tilesToRet[(i)*r + r - 1] = new Tile(wall[5]);
		}
		tilesToRet[(r/2)*r] = new Tile(wall[7]);
		tilesToRet[(r/2)*r + r - 1] = new Tile(wall[7]);
		tilesToRet[(0)*r+r/2] = new Tile(wall[6]);
		tilesToRet[(r-1)*r+r/2] = new Tile(wall[6]);

		if(r>7)
		{
			tilesToRet[(r / 2) * r + r / 2] = new Tile(allPossibleTiles[24]);

			tilesToRet[(r / 2) * r + r / 2 - 1] = new Tile(allPossibleTiles[15]);
			tilesToRet[(r / 2) * r + r / 2 + 1] = new Tile(allPossibleTiles[15]);
			tilesToRet[(r / 2 + 1) * r + r / 2] = new Tile(allPossibleTiles[15]);
			tilesToRet[(r / 2 - 1) * r + r / 2] = new Tile(allPossibleTiles[15]);
		}
	}
	std::vector<PossibleTiles*> possibleTiles;
	{
		PossibleTiles* posibleTile = new PossibleTiles();
		posibleTile->x = 1;
		posibleTile->y = 1;
		genPossibleTiles(tilesToRet, r, posibleTile);
		possibleTiles.push_back(posibleTile);
	}

	while (possibleTiles.size() > 0)
	{
		PossibleTiles *tile = possibleTiles.back();
		possibleTiles.pop_back();
		int t = tile->randTile();
		tilesToRet[tile->x + tile->y * r] = new Tile(allPossibleTiles[t]);
		genTile(tilesToRet, r, tile->x, tile->y + 1, possibleTiles);
		genTile(tilesToRet, r, tile->x, tile->y - 1, possibleTiles);
		genTile(tilesToRet, r, tile->x + 1, tile->y, possibleTiles);
		genTile(tilesToRet, r, tile->x - 1, tile->y, possibleTiles);
		delete tile;
		std::sort(possibleTiles.begin(), possibleTiles.end(), compareTiles);
	}


	return tilesToRet;
}