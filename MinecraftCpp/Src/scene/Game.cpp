#include "Game.h"
#include "../core/VBO.h"
#include "../core/EBO.h"
#include "../core/VAO.h"
#include "../core/Texture.h"
#include "../core/Engine.h"
#include "../core/Shader.h"
Game::Game(Camera* camera,GLFWwindow* window)
{

	Chunk::game = this;
	this->camera = camera;

	do {
		genWorld();
	} while (posToGenChunk.size() > 0);
	update(0.0f);
	this->window = window;
	glm::vec3 pos = camera->getPos();
	while (getBlockAt(pos.x,pos.y,pos.z))
	{
		pos.y++;
	}
	camera->newPos(pos);
	vao = new VAO();
	vboPos = new VBO();
	vboTex = new VBO();
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
		b = getBlockAt(x, y, z);
		if (b)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				deleteBlock(b);
			break;
		}
		n++;
	}
	toAddMutex.lock();
	for (auto a : toAdd)
		chunks.push_back(a);
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
		std::vector<GLuint> ind = b->getIndex();
		if (ind.size() > 0)
		{
			vao->bind();
			std::vector<glm::vec3> pos = b->getVertexPos();
			std::vector<glm::vec2> tex = b->getVertexTexture();
			vboPos->setNewVertices(pos);
			vboTex->setNewVertices(tex);
			ebo->setNewIndices(ind);
			ebo->bind();

			Shader& shader = getDiffoltShader();
			glUniform1i(shader.getUniformLocation("tex0"), 0);
			shader.setUniformVec2(glm::vec2(1, 1), "textSize");
			shader.active();
			selection->bind();
			glDisable(GL_DEPTH_TEST);
			glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);
			glEnable(GL_DEPTH_TEST);
		}
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
void Game::deleteBlock(Block* b)
{
	for (auto c : chunks)
		if (c->isThisChunk(b->x, b->y, b->z))
			c->deleteBlock(b);
}


void Game::setFaceing(int x, int y, int z, bool display, char face)
{
#define setFaceingDef(CheckingFace,face,x1,y1,z1) \
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
	setFaceingDef(Front, face, x, y, z - 1)
	setFaceingDef(Back, face, x, y, z + 1)
	setFaceingDef(Left, face, x + 1, y, z)
	setFaceingDef(Right, face, x - 1, y, z)
	setFaceingDef(Up, face, x, y - 1, z)
	setFaceingDef(Down, face, x, y + 1, z)

	setGenVerticesFlagAt(x, y, z);
}
void Game::setFaceing(Block* b, bool display, char face)
{
	if (!b)
		return;
	Block* block;
	if (checkFace(Front, face))
	{
		block = getBlockAt(b->x, b->y, b->z + 1);
		if (block)
		{
			b->setOneFace((int)Faces::Front, display);
			block->setOneFace((int)Faces::Back, display);
			setGenVerticesFlagAt(b->x, b->y, b->z + 1);
		}
	}
	if (checkFace(Back, face))
	{
		block = getBlockAt(b->x, b->y, b->z - 1);
		if (block)
		{
			block->setOneFace((int)Faces::Front, display);
			b->setOneFace((int)Faces::Back, display);
			setGenVerticesFlagAt(b->x, b->y, b->z - 1);
		}
	}
	if (checkFace(Up, face))
	{
		block = getBlockAt(b->x, b->y + 1, b->z);
		if (block)
		{
			b->setOneFace((int)Faces::Up, display);
			block->setOneFace((int)Faces::Down, display);
			setGenVerticesFlagAt(b->x, b->y + 1, b->z);
		}
	}
	if (checkFace(Down, face))
	{
		block = getBlockAt(b->x, b->y - 1, b->z);
		if (block)
		{
			block->setOneFace((int)Faces::Up, display);
			b->setOneFace((int)Faces::Down, display);
			setGenVerticesFlagAt(b->x, b->y - 1, b->z);
		}
	}
	if (checkFace(Left, face))
	{
		block = getBlockAt(b->x - 1, b->y, b->z);
		if (block)
		{
			b->setOneFace((int)Faces::Left, display);
			block->setOneFace((int)Faces::Right, display);
			setGenVerticesFlagAt(b->x - 1, b->y, b->z);
		}
	}
	if (checkFace(Right, face))
	{
		block = getBlockAt(b->x + 1, b->y, b->z);
		if (block)
		{
			block->setOneFace((int)Faces::Left, display);
			b->setOneFace((int)Faces::Right, display);
			setGenVerticesFlagAt(b->x + 1, b->y, b->z);
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
	glm::vec3 camPos = camera->getPos();
	while (gameRunning)
	{
		if (camPos != camera->getPos()|| posToGenChunk.size()>0)
		{
			genWorld();
		}
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

			for (int x = camPos.x - rangeX; x < camPos.x + rangeX; x++)
				for (int z = camPos.z - rangeZ; z < camPos.z + rangeZ; z++)
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
	float dist = glm::distance(pos, glm::vec2(pos.x, pos.y));

	if (dist <= range)
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
		if (glm::distance(glm::vec2(camPos.x, camPos.z), glm::vec2(cPos.x,cPos.z)) > range * 1.1f)
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
		c->save();

		glm::vec3 cPos = c->getLocation();
		if (glm::distance(glm::vec2(camPos.x, camPos.z), glm::vec2(cPos.x, cPos.z)) > range * 1.1f)
		{
			bool addToDelete = true;
			for (auto d : toDelete)
				if (c == d)
				{
					addToDelete = false;
					break;
				}

			if (addToDelete)
				toDelete.push_back(c);

		}

	}
	toSave.clear();
	chunksMutex.unlock();
	toDeleteMutex.unlock();

}