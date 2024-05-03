#include "WaveColapsFunction.h"
#include <vector>
#include <algorithm>
Tile::Tile(int ID, int up, int down, int left, int right, int rotate, bool bilding)
{
	this->ID = ID;
	this->up = up;
	this->down = down;
	this->left = left;
	this->right = right;
	this->rotate = rotate % 4;
	this->bilding = bilding;
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
		if(!bilding || !tile.bilding)
			return tile.down == up;
		return up == 0 && tile.down == up;
	case Dir::Down:
		if (!bilding || !tile.bilding)
			return tile.up == down;
		return down == 0 && tile.up == down;
	case Dir::Left:
		if (!bilding || !tile.bilding)
			return tile.right == left;
		return left == 0 && tile.right == left;
	case Dir::Right:
		if (!bilding || !tile.bilding)
			return tile.left == right;
		return right == 0 && tile.left == right;
	}
	return false;	
}

static std::vector<Tile> allPosibleTiles{

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

struct PosiblesTiles
{
	int x, y;
	std::vector<int> tiles;

	int randTile()
	{
		return tiles[rand() % tiles.size()];
	}
};

static bool compareTiles(PosiblesTiles* t1, PosiblesTiles* t2)
{
	return t1->tiles.size() < t2->tiles.size();
}

static Tile* getTileAt(Tile** tiles, int r, int x, int y)
{
	if (x < 0 || x >= r || y < 0 || y >= r)
		return NULL;
	return tiles[x + y * r];
}

static void genPosibleTiles(Tile** tiles, int r, PosiblesTiles* t)
{
	std::vector<int> posibleID;
	Tile* tileUp = getTileAt(tiles, r, t->x, t->y - 1);
	Tile* tileDown = getTileAt(tiles, r, t->x, t->y + 1);
	Tile* tileLeft = getTileAt(tiles, r, t->x - 1, t->y);
	Tile* tileRight = getTileAt(tiles, r, t->x + 1, t->y);
	for (int i = 0; i < allPosibleTiles.size(); i++)
	{
		posibleID.push_back(i);
	}
	if (tileUp)
	{
		std::vector<int> posibleIDTmp;
		for(auto p:posibleID)
		{
			if (tileUp->canConnect(Dir::Down, allPosibleTiles[p]))
				posibleIDTmp.push_back(p);
		}
		posibleID = posibleIDTmp;
	}
	if (tileDown)
	{
		std::vector<int> posibleIDTmp;
		for (auto p : posibleID)
		{
			if (tileDown->canConnect(Dir::Up, allPosibleTiles[p]))
				posibleIDTmp.push_back(p);
		}
		posibleID = posibleIDTmp;
	}
	if (tileLeft)
	{
		std::vector<int> posibleIDTmp;
		for (auto p : posibleID)
		{
			if (tileLeft->canConnect(Dir::Right, allPosibleTiles[p]))
				posibleIDTmp.push_back(p);
		}
		posibleID = posibleIDTmp;
	}
	if (tileRight)
	{
		std::vector<int> posibleIDTmp;
		for (auto p : posibleID)
		{
			if (tileRight->canConnect(Dir::Left, allPosibleTiles[p]))
				posibleIDTmp.push_back(p);
		}
		posibleID = posibleIDTmp;
	}
	if (posibleID.size() <= 0)
		posibleID.push_back(0);
	t->tiles = posibleID;
}

static bool hasPosibleTileAt(int x, int y, std::vector<PosiblesTiles*> posibleTiles)
{
	for (auto p : posibleTiles)
		if (p->x == x && p->y == y)
			return true;
	return false;
}

static PosiblesTiles* getPosibleTileAt(int x,int y, std::vector<PosiblesTiles*> posibleTiles)
{
	for (auto p : posibleTiles)
		if (p->x == x && p->y == y)
			return p;
	return NULL;
}

static void genTile(Tile** tiles, int &r,int x,int y, std::vector<PosiblesTiles*>& posibleTiles)
{
	if (x < 0 || x >= r || y < 0 || y >= r)
		return;
	//int circlePoint = r / 2 + r % 2;
	//if (pow(x - circlePoint, 2) + pow(y - circlePoint, 2) > pow(r / 2, 2))
	//	return;
	if (!getTileAt(tiles, r, x, y))
	{
		if (hasPosibleTileAt(x, y, posibleTiles))
		{
			PosiblesTiles* t = getPosibleTileAt(x, y, posibleTiles);
			if(t)
				genPosibleTiles(tiles, r, t);
		}
		else
		{
			PosiblesTiles* t = new PosiblesTiles();
			t->x = x;
			t->y = y;
			genPosibleTiles(tiles, r, t);
			posibleTiles.push_back(t);
		}
	}
}

Tile** generateVilage(int r)
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

	}
	std::vector<PosiblesTiles*> posibleTiles;
	{
		PosiblesTiles* posibleTile = new PosiblesTiles();
		posibleTile->x = 1;
		posibleTile->y = 1;
		genPosibleTiles(tilesToRet, r, posibleTile);
		posibleTiles.push_back(posibleTile);
	}

	while (posibleTiles.size() > 0)
	{
		PosiblesTiles *tile = posibleTiles.back();
		posibleTiles.pop_back();
		int t = tile->randTile();
		tilesToRet[tile->x + tile->y * r] = new Tile(allPosibleTiles[t]);
		genTile(tilesToRet, r, tile->x, tile->y + 1, posibleTiles);
		genTile(tilesToRet, r, tile->x, tile->y - 1, posibleTiles);
		genTile(tilesToRet, r, tile->x + 1, tile->y, posibleTiles);
		genTile(tilesToRet, r, tile->x - 1, tile->y, posibleTiles);
		delete tile;
		std::sort(posibleTiles.begin(), posibleTiles.end(), compareTiles);
	}


	return tilesToRet;
}