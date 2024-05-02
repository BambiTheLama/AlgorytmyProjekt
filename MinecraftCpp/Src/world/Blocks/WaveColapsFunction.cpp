#include "WaveColapsFunction.h"
#include <vector>
#include <algorithm>
Tile::Tile(int ID, int up, int down, int left, int right, int rotate)
{
	this->ID = ID;
	this->up = up;
	this->down = down;
	this->left = left;
	this->right = right;
	this->rotate = rotate % 4;
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

bool Tile::canConnect(Dir dir, Tile& tile)
{
	switch (dir)
	{
	case Dir::Up:
		return tile.down == up;
	case Dir::Down:
		return tile.up == down;
	case Dir::Left:
		return tile.right == left;
	case Dir::Right:
		return tile.left == right;
	}
	return false;	
}

static std::vector<Tile> allPosibleTiles{
	Tile(),

	Tile(1,1),
	Tile(1,1,0,0,0,1),
	Tile(1,1,0,0,0,2),
	Tile(1,1,0,0,0,3),

	Tile(2,1,1),
	Tile(2,1,1,0,0,1),

	Tile(3,1,1,1,0,0),
	Tile(3,1,1,1,0,1),
	Tile(3,1,1,1,0,2),
	Tile(3,1,1,1,0,3),

	Tile(4,0,1,1,0,0),
	Tile(4,0,1,1,0,1),
	Tile(4,0,1,1,0,2),
	Tile(4,0,1,1,0,3),

	Tile(5,1,1,1,1,0),
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

static Tile* getTileAt(Tile** tiles, int w, int h, int x, int y)
{
	if (x < 0 || x >= w || y < 0 || y >= h)
		return NULL;
	return tiles[x + y * w];
}

static void genPosibleTiles(Tile** tiles, int w, int h, PosiblesTiles* t)
{
	std::vector<int> posibleID;
	Tile* tileUp = getTileAt(tiles, w, h, t->x, t->y - 1);
	Tile* tileDown = getTileAt(tiles, w, h, t->x, t->y + 1);
	Tile* tileLeft = getTileAt(tiles, w, h, t->x - 1, t->y);
	Tile* tileRight = getTileAt(tiles, w, h, t->x + 1, t->y);

	for (int i = 0; i < allPosibleTiles.size(); i++)
	{
		posibleID.push_back(i);
	}
	if (tileUp)
	{
		std::vector<int> posibleIDTmp;
		for(auto p:posibleID)
		{
			if (allPosibleTiles[p].up == tileUp->down)
				posibleIDTmp.push_back(p);
		}
		posibleID = posibleIDTmp;
	}
	if (tileDown)
	{
		std::vector<int> posibleIDTmp;
		for (auto p : posibleID)
		{
			if (allPosibleTiles[p].down == tileDown->up)
				posibleIDTmp.push_back(p);
		}
		posibleID = posibleIDTmp;
	}
	if (tileLeft)
	{
		std::vector<int> posibleIDTmp;
		for (auto p : posibleID)
		{
			if (allPosibleTiles[p].left == tileLeft->right)
				posibleIDTmp.push_back(p);
		}
		posibleID = posibleIDTmp;
	}
	if (tileRight)
	{
		std::vector<int> posibleIDTmp;
		for (auto p : posibleID)
		{
			if (allPosibleTiles[p].right == tileRight->left)
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

static void genTile(Tile** tiles, int &w, int &h,int x,int y, std::vector<PosiblesTiles*>& posibleTiles)
{
	if (x < 0 || x >= w || y < 0 || y >= h)
		return;
	if (!getTileAt(tiles, w, h, x, y))
	{
		if (hasPosibleTileAt(x, y, posibleTiles))
		{
			PosiblesTiles* t = getPosibleTileAt(x, y, posibleTiles);
			if(t)
				genPosibleTiles(tiles, w, h, t);
		}
		else
		{
			PosiblesTiles* t = new PosiblesTiles();
			t->x = x;
			t->y = y;
			genPosibleTiles(tiles, w, h, t);
			posibleTiles.push_back(t);
		}
	}
}

Tile** generateVilage(int w, int h)
{
	Tile** tilesToRet = new Tile*[w * h];
	for (int i = 0; i < w * h; i++)
		tilesToRet[i] = NULL;

	std::vector<PosiblesTiles*> posibleTiles;
	{
		PosiblesTiles* posibleTile = new PosiblesTiles();
		posibleTile->x = 0;
		posibleTile->y = 0;
		genPosibleTiles(tilesToRet, w, h, posibleTile);
		posibleTiles.push_back(posibleTile);
	}

	while (posibleTiles.size() > 0)
	{
		PosiblesTiles *tile = posibleTiles.back();
		posibleTiles.pop_back();
		int t = tile->randTile();
		tilesToRet[tile->x + tile->y * w] = new Tile(allPosibleTiles[t]);
		genTile(tilesToRet, w, h, tile->x, tile->y + 1, posibleTiles);
		genTile(tilesToRet, w, h, tile->x, tile->y - 1, posibleTiles);
		genTile(tilesToRet, w, h, tile->x + 1, tile->y, posibleTiles);
		genTile(tilesToRet, w, h, tile->x - 1, tile->y, posibleTiles);
		delete tile;
		std::sort(posibleTiles.begin(), posibleTiles.end(), compareTiles);
	}




	return tilesToRet;
}