#pragma once

enum class Dir
{
	Up,Down,Left,Right
};

struct Tile
{	
	int ID;
	int up, down, left, right;
	int rotate = 0;
	bool building;
	Tile(int ID = 0, int up = 0, int down = 0, int left = 0, int right = 0, int rotate = 0,bool building=false);

	bool canConnect(Dir dir, Tile tile);
private:

	void rotateTile();

};


Tile** generateVillage(int r);