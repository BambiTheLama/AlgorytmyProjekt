#include "Game.h"

Game::Game(Camera* camera,GLFWwindow* window)
{
	int t = 10;
	int h = 5;
	int w = 10;
	Chunk::game = this;
	this->camera = camera;
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			for (int k = 0; k < t; k++)
				chunks.push_back(new Chunk(i, j, k));
	//chunks.push_back(new Chunk(0, 0, 0));
	this->window = window;
	glm::vec3 pos = camera->getPos();
	while (getBlockAt(pos.x,pos.y,pos.z))
	{
		pos.y++;
	}
	camera->newPos(pos);

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

	glm::vec3 pos = camera->getPos();
	glm::vec3 dir = camera->getDir();
	int n = 0;
	while (n < 100)
	{
		int x = pos.x + n * dir.x;
		int y = pos.y + n * dir.y;
		int z = pos.z + n * dir.z;
		b = getBlockAt(x, y, z);
		if (b)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				deleteBlock(b);
			break;
		}
		n++;
	}
	

	
}

void Game::draw()
{
	for (auto c : chunks)
	{
		float dist =glm::distance(camera->getPos(), glm::vec3(c->x * chunkW, c->y * chunkH, c->z * chunkT));
		if(dist<camera->getRange()*1.5f)
			c->draw();
	}

	if (b)
	{

	}

}

Block* Game::getBlockAt(int x, int y, int z)
{
	for (auto c : chunks)
		if (c->isThisChunk(x, y, z))
			return c->getBlock(x, y, z);
	return NULL;
}
void Game::deleteBlock(Block* b)
{
	for (auto c : chunks)
		if (c->isThisChunk(b->x, b->y, b->z))
			c->deleteBlock(b);
}


void Game::setFaceing(int x, int y, int z, bool display)
{
	Block* block = getBlockAt(x, y + 1, z);
	if (block)
	{
		block->setOneFace((int)Faces::Down, display);
	}
	block = getBlockAt(x, y - 1, z);
	if (block)
	{
		block->setOneFace((int)Faces::Up, display);
	}
	block = getBlockAt(x, y, z + 1);
	if (block)
	{
		block->setOneFace((int)Faces::Back, display);
	}
	block = getBlockAt(x, y, z - 1);
	if (block)
	{
		block->setOneFace((int)Faces::Front, display);
	}
	block = getBlockAt(x + 1, y, z);
	if (block)
	{
		block->setOneFace((int)Faces::Left, display);
	}
	block = getBlockAt(x - 1, y, z);
	if (block)
	{
		block->setOneFace((int)Faces::Right, display);
	}
}
void Game::setFaceing(Block* b, bool display)
{
	if (!b)
		return;
	Block* block = getBlockAt(b->x, b->y + 1, b->z);
	if (block)
	{
		b->setOneFace((int)Faces::Up, display);
		block->setOneFace((int)Faces::Down, display);
	}
	block = getBlockAt(b->x, b->y - 1, b->z);
	if (block)
	{
		block->setOneFace((int)Faces::Up, display);
		b->setOneFace((int)Faces::Down, display);
	}
	block = getBlockAt(b->x, b->y, b->z + 1);
	if (block)
	{
		b->setOneFace((int)Faces::Front, display);
		block->setOneFace((int)Faces::Back, display);
	}
	block = getBlockAt(b->x, b->y, b->z - 1);
	if (block)
	{
		block->setOneFace((int)Faces::Front, display);
		b->setOneFace((int)Faces::Back, display);
	}
	block = getBlockAt(b->x + 1, b->y, b->z);
	if (block)
	{
		block->setOneFace((int)Faces::Left, display);
		b->setOneFace((int)Faces::Right, display);
	}
	block = getBlockAt(b->x - 1, b->y, b->z);
	if (block)
	{
		b->setOneFace((int)Faces::Left, display);
		block->setOneFace((int)Faces::Right, display);
	}
}