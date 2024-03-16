#include "Game.h"

Game::Game(Camera* camera,GLFWwindow* window)
{

	Chunk::game = this;
	this->camera = camera;
	do {
		genWorld();
	} while (posToGenChunk.size() > 0);

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
	gameRunning = false;
	if (worldGenerateT.joinable())
		worldGenerateT.join();
	Chunk::game = NULL;
	for (auto c : chunks)
		delete c;
	chunks.clear();
}

void Game::start()
{
	gameRunning = true;
	worldGenerateT = std::thread(&Game::worldGenerateFun, this);
}

void Game::update(float deltaTime)
{

	chunksMutex.lock();
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
	chunksMutex.unlock();

}

void Game::draw()
{
	chunksMutex.lock();
	for (auto c : chunks)
	{
		float dist =glm::distance(camera->getPos(), glm::vec3(c->x * chunkW, c->y * chunkH, c->z * chunkT));
		if(dist<camera->getRange()*1.5f)
			c->draw();
	}

	if (b)
	{

	}
	chunksMutex.unlock();

}
void Game::endPhase()
{
	chunksMutex.lock();
	toAddMutex.lock();
	for (auto a : toAdd)
		chunks.push_back(a);
	toAdd.clear();
	chunksMutex.unlock();
	toAddMutex.unlock();
}
Block* Game::getBlockAt(int x, int y, int z)
{
	for (auto c : chunks)
		if (c->isThisChunk(x, y, z))
		{
			return c->getBlock(x, y, z);
		}
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

void Game::worldGenerateFun()
{
	while (gameRunning)
	{
		genWorld();
	}
}

void Game::genWorld()
{
	glm::vec3 camPos = camera->getPos();
	camPos.x /= chunkW;
	camPos.y /= chunkH;
	camPos.z /= chunkT;
	if (posToGenChunk.size() <= 0)
	{
		for (int y = camPos.y - range; y < camPos.y + range; y++)
			for (int x = camPos.x - range; x < camPos.x + range; x++)
				for (int z = camPos.z - range; z < camPos.z + range; z++)
				{
					bool breked = false;
					toAddMutex.lock();
					for (auto c : toAdd)
						if (c->y == y && c->z == z && c->x == x)
						{
							breked = true;
							break;
						}
					toAddMutex.unlock();
					chunksMutex.lock();
					for (auto c : chunks)
						if (c->y == y && c->z == z && c->x == x)
						{
							breked = true;
							break;
						}
					if (!breked)
						posToGenChunk.push_back(glm::vec3(x, y, z));
					chunksMutex.unlock();
				}
	}
	if (posToGenChunk.size() <= 0)
		return;
	glm::vec3 pos = posToGenChunk.back();
	posToGenChunk.pop_back();
	toAddMutex.lock();
	bool breked = false;
	for (auto c : toAdd)
		if (c->y == pos.y && c->z == pos.z && c->x == pos.x)
		{
			breked = true;
			break;
		}
	toAddMutex.unlock();
	Chunk* c = NULL;
	float dist = glm::distance(pos, camPos);

	if (dist <= range * 1.5f)
	{
		c = new Chunk(pos.x, pos.y, pos.z);
	}

	toAddMutex.lock();
	if(c)
		toAdd.push_back(c);
	toAddMutex.unlock();
}