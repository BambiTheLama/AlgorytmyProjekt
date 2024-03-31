#include "Game.h"
#include "../core/VBO.h"
#include "../core/EBO.h"
#include "../core/VAO.h"
#include "../core/Texture.h"
#include "../core/Engine.h"
#include "../core/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
Game::Game(Camera* camera,GLFWwindow* window)
{
	setNoiseSeed(123693543);
	Chunk::game = this;
	this->camera = camera;
	cube = new Cube();



	this->window = window;

	vao = new VAO();
	vbo = new VBO();
	ebo = new EBO();
	selection = new Texture("Res/Selected.jpg");
}

Game::~Game()
{
	gameRunning = false;
	if (worldGenerateT.joinable())
		worldGenerateT.join();
	if (worldDestroyT.joinable())
		worldDestroyT.join();
	Chunk::game = NULL;
	for (auto c : toAdd)
		delete c;
	for (auto c : chunks)
	{
		c->save();
		delete c;
	}

	chunks.clear();
}

void Game::start()
{
	gameRunning = true;
	worldGenerateT = std::thread(&Game::worldGenerateFun, this);
	worldDestroyT  = std::thread(&Game::worldDestroyFun, this);
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
		Block* b2 = getBlockAt(x, y, z);
		if (b2 && b == b2)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				deleteBlock(x, y, z);
				b = NULL;

			}
			break;
		}

		if (b2 && b != b2)
		{
			b = b2;
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				deleteBlock(x, y, z);
				b = NULL;
				break;
			}
			chunkPos = getChunkPos(x, y, z);
			cube->setFaceing(b->getFaces());
			vertices = cube->getVertex(x % chunkW, y % chunkH, z % chunkT, 1, 0, 0);
			index = cube->getIndex();
			vao->bind();
			ebo->setNewVertices(index);
			vbo->setNewVertices(vertices);
			vao->linkData(*vbo, 0, 1, GL_FLOAT, sizeof(int), (void*)0);
			break;
		}
		n++;
	}
	toAddMutex.lock();
	for (auto a : toAdd)
	{
		a->start();
		a->update(0.0f);
		chunks.push_back(a);

	}

	toAdd.clear();
	toAddMutex.unlock();
	toDeleteMutex.lock();
	for (auto d : toDelete)
	{
		int r = -1;
		for (int i=0;i<chunks.size();i++)
		{
			if (chunks[i] == d)
			{
				r = i;
				delete d;
				break;
			}
		}
		if (r >= 0 && r < chunks.size())
			chunks.erase(chunks.begin() + r);
	}
	toDelete.clear();
	toDeleteMutex.unlock();
	toDraw.clear();
	for (auto c : chunks)
	{
		toDraw.push_back(c);
	}
	for (int i = 0; i < toDraw.size(); i++)
	{
		for (int j = i; j < toDraw.size(); j++)
		{
			if (glm::distance(toDraw[i]->getPos(), camera->getPos()) < glm::distance(toDraw[j]->getPos(), camera->getPos()))
			{
				Chunk* c = toDraw[i];
				toDraw[i] = toDraw[j];
				toDraw[j] = c;
			}
		}
	}
	chunksMutex.unlock();

}

void Game::draw(Shader* s)
{
	chunksMutex.lock();
	for (auto c : toDraw)
	{
		s->setUniformVec4(glm::vec4(1, 1, 1, 1), "modelColor");
		c->draw(s);
	}
	if (b)
	{
		vao->bind();
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(chunkPos.x,0, chunkPos.z));
		s->setUniformMat4(model, "model");

		printf("%lf %lf %lf\n", chunkPos.x, 0, chunkPos.z);

		glDisable(GL_DEPTH_TEST);
		s->setUniformVec4(glm::vec4(10, 0, 10, 0.6f), "modelColor");
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		s->setUniformVec4(glm::vec4(10, 10, 10, 1), "modelColor");
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);

	}
	chunksMutex.unlock();

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

glm::vec3 Game::getChunkPos(int x, int y, int z)
{
	for (auto c : chunks)
		if (c->isThisChunk(x, y, z))
		{
			glm::vec3 pos = c->getLocation();
			return glm::vec3(pos.x * chunkW, pos.y * chunkH, pos.z * chunkT);
		}
	return glm::vec3(0, 0, 0);
}

void Game::deleteBlock(int x,int y,int z)
{
	for (auto c : chunks)
		if (c->isThisChunk(x, y, z))
			c->deleteBlock(x,y,z);
}

void Game::setFaceing(int x, int y, int z, bool display, char face)
{
#define setFaceingDef(CheckingFace,x1,y1,z1) \
	if (checkFace(CheckingFace, face))\
	{\
		block = getBlockAt(x1, y1, z1);\
		if (block)\
		{\
			block->setOneFace((int)Faces::CheckingFace, display);\
			setGenVerticesFlagAt(x1, y1, z1);\
		}\
	}
					

	Block* block;
	setFaceingDef(Front, x, y, z - 1)
	setFaceingDef(Back, x, y, z + 1)
	setFaceingDef(Left, x + 1, y, z)
	setFaceingDef(Right, x - 1, y, z)
	setFaceingDef(Up, x, y - 1, z)
	setFaceingDef(Down, x, y + 1, z)

	setGenVerticesFlagAt(x, y, z);
}

void Game::setFaceing(Block* b, int x, int y, int z, char face)
{
	if (!b)
		return;
	Block* block;
	bool display;
	if (checkFace(Front, face))
	{
		block = getBlockAt(b->x + x, b->y + y, b->z + 1 + z);
		if (block)
		{
			display = b->isTransparent() != block->isTransparent();
			b->setOneFace((int)Faces::Front, display);
			block->setOneFace((int)Faces::Back, display);
			setGenVerticesFlagAt(b->x + x, b->y + y, b->z + 1 + z);
		}
	}
	if (checkFace(Back, face))
	{
		block = getBlockAt(b->x + x, b->y + y, b->z - 1 + z);
		if (block)
		{
			display = b->isTransparent() != block->isTransparent();
			block->setOneFace((int)Faces::Front, display);
			b->setOneFace((int)Faces::Back, display);
			setGenVerticesFlagAt(b->x + x, b->y + y, b->z - 1 + z);
		}
	}
	if (checkFace(Up, face))
	{
		block = getBlockAt(b->x + x, b->y + y + 1, b->z  + z);
		if (block)
		{
			display = b->isTransparent() != block->isTransparent();
			b->setOneFace((int)Faces::Up, display);
			block->setOneFace((int)Faces::Down, display);
			setGenVerticesFlagAt(b->x + x, b->y + y + 1, b->z + z);
		}
	}
	if (checkFace(Down, face))
	{
		block = getBlockAt(b->x + x, b->y + y - 1, b->z + z);
		if (block)
		{
			display = b->isTransparent() != block->isTransparent();
			block->setOneFace((int)Faces::Up, display);
			b->setOneFace((int)Faces::Down, display);
			setGenVerticesFlagAt(b->x + x, b->y + y - 1, b->z + z);
		}
	}
	if (checkFace(Left, face))
	{
		block = getBlockAt(b->x + x - 1, b->y + y , b->z + z);
		if (block)
		{
			display = b->isTransparent() != block->isTransparent();
			b->setOneFace((int)Faces::Left, display);
			block->setOneFace((int)Faces::Right, display);
			setGenVerticesFlagAt(b->x + x - 1, b->y + y, b->z + z);
		}
	}
	if (checkFace(Right, face))
	{
		block = getBlockAt(b->x + x + 1, b->y + y, b->z + z);
		if (block)
		{
			display = b->isTransparent() != block->isTransparent();
			block->setOneFace((int)Faces::Left, display);
			b->setOneFace((int)Faces::Right, display);
			setGenVerticesFlagAt(b->x + x + 1, b->y + y, b->z + z);
		}
	}

	setGenVerticesFlagAt(b->x, b->y, b->z);
}

void Game::setGenVerticesFlagAt(int x, int y, int z)
{
	for (auto c : chunks)
		if (c->isThisChunk(x, y, z))
		{
			c->genVerticesFlag();
			return;
		}
}

void Game::worldGenerateFun()
{
	glm::vec3 camPos = glm::vec3(0.0f);
	while (gameRunning)
	{
		if (camPos != camera->getPos() || posToGenChunk.size() > 0)
		{
			genWorld();
		}
		else if (posToGenChunk.size() <= 0)
			camPos = camera->getPos();


	}
}

void Game::worldDestroyFun()
{
	glm::vec3 camPos = camera->getPos();
	while (gameRunning)
	{
		if (camPos != camera->getPos() || posToGenChunk.size() > 0)
		{
			desWorld();
		}
		camPos = camera->getPos();

	}
}

void Game::genWorld()
{
	glm::vec3 camPos = camera->getPos();
	camPos.x /= chunkW;
	camPos.z /= chunkT;
	if (posToGenChunk.size() <= 0)
	{

			for (int x = camPos.x - range; x < camPos.x + range; x++)
				for (int z = camPos.z - range; z < camPos.z + range; z++)
				{
					bool breked = false;
					toAddMutex.lock();
					for (auto c : toAdd)
						if (c->z == z && c->x == x)
						{
							breked = true;
							break;
						}
					toAddMutex.unlock();
					chunksMutex.lock();
					for (auto c : chunks)
						if (c->z == z && c->x == x)
						{
							breked = true;
							break;
						}
					if (!breked)
						posToGenChunk.push_back(glm::vec2(x, z));
					chunksMutex.unlock();
				}
	}
	if (posToGenChunk.size() <= 0)
		return;
	glm::vec2 pos = posToGenChunk.back();
	posToGenChunk.pop_back();
	toAddMutex.lock();
	bool breked = false;
	for (auto c : toAdd)
		if (c->z == pos.y && c->x == pos.x)
		{
			breked = true;
			break;
		}
	toAddMutex.unlock();
	Chunk* c = NULL;

	if (abs(pos.x - camPos.x) <= range && abs(pos.y - camPos.z) <= range)
	{
		c = new Chunk(pos.x, 0, pos.y);
	}

	toAddMutex.lock();
	if(c)
		toAdd.push_back(c);
	toAddMutex.unlock();
}

void Game::desWorld()
{
	glm::vec3 camPos = camera->getPos();
	camPos.x /= chunkW;
	camPos.y /= chunkH;
	camPos.z /= chunkT;
	
	chunksMutex.lock();
	for (auto c : chunks)
	{
		glm::vec3 cPos = c->getLocation();
		if (abs(cPos.x - camPos.x) > range * 1.5 || abs(cPos.y - camPos.z) > range * 1.5)
		{
			toSave.push_back(c);
		}
	}

	toDeleteMutex.lock();
	camPos = camera->getPos();
	camPos.x /= chunkW;
	camPos.y /= chunkH;
	camPos.z /= chunkT;
	for (auto c : toSave)
	{
		glm::vec3 cPos = c->getLocation();
		if (glm::distance(glm::vec2(camPos.x, camPos.z), glm::vec2(cPos.x, cPos.z)) > range * 1.5f)
		{
			bool addToDelete = true;
			for (auto d : toDelete)
				if (c == d)
				{
					addToDelete = false;
					break;
				}

			if (addToDelete)
			{
				c->save();
				c->clearBlocks();
				toDelete.push_back(c);
			}


		}

	}
	toSave.clear();
	chunksMutex.unlock();
	toDeleteMutex.unlock();

}