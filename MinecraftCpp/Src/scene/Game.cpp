#include "Game.h"

Game::Game(Camera* camera)
{
	int h = 2;
	int w = 2;
	Chunk::game = this;
	this->camera = camera;
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			chunks.push_back(new Chunk(i-w/2, -1, j-h/2));
	chunks.push_back(new Chunk(0, 0, 0));
}

Game::~Game()
{
	Chunk::game = NULL;
	for (auto c : chunks)
		delete c;
	chunks.clear();
}

void Game::update(float deltaTime)
{
	for (auto c : chunks)
		c->update(deltaTime);
	sortChunks();
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

void Game::sortChunks()
{
	glm::vec3 pos = camera->getPos();

	std::vector<float> dist;
	for (auto c : chunks)
	{
		dist.push_back(glm::distance(pos, c->getPos()));
	}
	for (int i = 0; i < dist.size(); i++)
	{
		for (int j = i + 1; j < dist.size(); j++)
		{
			if (dist[j] > dist[i])
			{
				float d = dist[j];
				dist[j] = dist[i];
				dist[i] = d;
				Chunk* c = chunks[i];
				chunks[i] = chunks[j];
				chunks[j] = c;
			}
		}
	}
}