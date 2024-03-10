#include "Game.h"

Game::Game()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			chunks.push_back(new Chunk(i, -1, j));
}

Game::~Game()
{
	for (auto c : chunks)
		delete c;
	chunks.clear();
}

void Game::update(float deltaTime)
{
	for (auto c : chunks)
		c->update(deltaTime);
}

void Game::draw()
{
	for (auto c : chunks)
		c->draw();
}

Block* Game::getBlockAt(int x, int y, int z)
{
	for (auto c : chunks)
		if (c->isThisChunk(x, y, z))
			return c->getBlock(x, y, z);
	return NULL;
}