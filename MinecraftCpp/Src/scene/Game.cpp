#include "Game.h"
#include "../core/VBO.h"
#include "../core/EBO.h"
#include "../core/VAO.h"
#include "../core/Texture.h"
#include "../core/Engine.h"
#include "../core/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../core/RenderTexture.h"
#include <algorithm>

Game* Game::game = NULL;

Game::Game(int w,int h,GLFWwindow* window)
{
	Chunk::game = this;
	cube = new Cube();



	this->window = window;

	vao = new VAO();
	vbo = new VBO();
	ebo = new EBO();
	selection = new Texture("Res/Selected.jpg");


	shader = new Shader("Shader/Diff.vert", "Shader/Diff.frag");
	shaderShadow = new Shader("Shader/Shadow.vert", "Shader/Shadow.frag");

	blocksN = new GameTextures("Res/BlocksN/");
	blocksH = new GameTextures("Res/BlocksH/");
	blocks  = new GameTextures("Res/Blocks/");
	shader->active();
	blocks ->setTextures(*shader, "tex0");
	blocksN->setTextures(*shader, "texN");
	blocksH->setTextures(*shader, "texH");
	glm::mat4 modelMat = glm::mat4(1.0f);
	shader->setUniformMat4(modelMat, "model");
	shader->setUniformVec4(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "modelColor");
	ShadowMap = new RenderTexture(2048, 2048);
	ShadowMap->use(*shader, "texShadow");
	camera = new Camera(w, h, 0.1f, 1000, 60, glm::vec3(0.0f, 100.0f, -1.0f));
	game = this;

	for (int i = 0; i < 6; i++)
	{
		solidMesh[i] = new ChunkMesh(i);
		transMesh[i] = new ChunkMesh(i);
	}
	for (int i = 6; i < 10; i++)
		transMesh[i] = new ChunkMesh(i);

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
	for (int i = 0; i < 6; i++)
	{
		delete solidMesh[i];
		delete transMesh[i];
	}
	for (int i = 6; i < 10; i++)
		delete transMesh[i];

	delete shader;
	delete blocks;
	delete blocksH;
	delete blocksN;
	delete shaderShadow;
	game = NULL;
	Chunk::saveBlockData();
}

void Game::start()
{

	gameRunning = true;
	worldGenerateT = std::thread(&Game::worldGenerateFun, this);
	worldDestroyT  = std::thread(&Game::worldDestroyFun, this);
	

}

glm::vec3 camPos;

bool compareObj(Chunk* c1, Chunk* c2)
{
	return glm::distance(c1->getPos(), camPos) > glm::distance(c2->getPos(), camPos);
}

void Game::update(float deltaTime)
{
	time += deltaTime;
	camera->update(window, deltaTime);
	chunksMutex.lock();
	toAddMutex.lock();
	for (auto c : chunks)
		c->update(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		debug = false;
	}
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		debug = true;
	}
	toAddMutex.unlock();
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
	if (toAdd.size() > 0)
		reloadMesh = true;
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
	if (toDelete.size() > 0)
	{
		reloadMesh = true;
	}
	toDelete.clear();


	toDeleteMutex.unlock();

	toDraw = chunks;

	camPos = camera->getPos();
	std::sort(toDraw.begin(), toDraw.end(), compareObj);
	if (glfwGetKey(window, GLFW_KEY_TAB))
		for (auto c : chunks)
		{
			c->setFaceing();
			c->genVerticesFlag();
		}

	chunksMutex.unlock();



}

void Game::draw()
{
	if (reloadMesh)
		genMesh();
	camera->useCamera(*shader, "camera");

	glm::mat4 model(1.0f);
	shader->setUniformMat4(model, "model");

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	ShadowMap->startUse();

	cameraDir = camera->getDir();
	cameraPos = camera->getPos();
	glm::vec3 lightDir = glm::vec3(sin(time), 1.0f, sin(time / 8) / 2);
	glm::vec3 shadowMapLightDir = -lightDir;
	camera->setDir(shadowMapLightDir);

	camera->setUseProjection(false);
	camera->newPos(glm::vec3(((int)cameraPos.x / 16) * 16, 255, ((int)cameraPos.z / 16) * 16));

	shaderShadow->active();
	camera->useCamera(*shaderShadow, "camera");
	renderScene(shaderShadow, true);
	ShadowMap->endUse();
	shader->active();
	shader->setUniformI1(debug, "isDebug");
	glm::mat4 modelMat(1.0f);
	shader->setUniformMat4(modelMat, "model");
	shader->setUniformVec3(camera->getPos(), "camPos");
	shader->setUniformVec3(glm::vec3(1.0f, 1.0f, 1.0f), "lightColor");
	camera->useCamera(*shader, "lightProjection");
	shader->setUniformVec3(lightDir, "lightDir");
	shader->setUniformVec3(shadowMapLightDir, "shadowMapLightDir");

	ShadowMap->use(*shader, "texShadow");
	blocks->bindTextures();
	blocksN->bindTextures();
	blocksH->bindTextures();
	


	

	camera->setDir(cameraDir);
	camera->newPos(cameraPos);
	camera->setUseProjection(true);
	camera->useCamera(*shader, "camera");
	renderScene(shader,true);


	drawBlock(shader);
	ShadowMap->draw();

	
}

void Game::renderScene(Shader* s,bool trans)
{
	chunksMutex.lock();
	glEnable(GL_DEPTH_TEST);
	s->setUniformVec4(glm::vec4(1, 1, 1, 1), "modelColor");
	for (auto c : toDraw)
	{
		c->draw(s, trans);
	}
	for (int i = 0; i < 6; i++)
	{
		solidMesh[i]->draw(s);
	}
	for (int i = 0; i < 10; i++)
	{
		transMesh[i]->draw(s);
	}
	glDisable(GL_DEPTH_TEST);
	chunksMutex.unlock();
}

void Game::drawBlock(Shader* s)
{
	if (b)
	{
		vao->bind();
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(chunkPos.x, 0, chunkPos.z));
		s->setUniformMat4(model, "model");

		//glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_EQUAL);
		s->setUniformVec4(glm::vec4(100, 0, 100, 0.6f), "modelColor");
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		s->setUniformVec4(glm::vec4(100, 100, 100, 1), "modelColor");
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthFunc(GL_LESS);
		//glEnable(GL_DEPTH_TEST);

	}
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

bool Game::addBlock(Block* b)
{
	int x = b->x;
	int y = b->y;
	int z = b->z;
	for (auto c : chunks)
		if (c->isThisChunk(x, y, z))
			return c->addBlock(b);
	for (auto a : toAdd)
		if (a->isThisChunk(x, y, z))
			return a->addBlock(b);
	Chunk::saveBlockToChunk(x, y, z, b->ID);
	return false;
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

void Game::setFaceing(Block* b, char face)
{
	if (!b)
		return;
	Block* block;
	bool display;
	if (checkFace(Front, face))
	{
		block = getBlockAt(b->x, b->y, b->z + 1);
		if (block)
		{
			display = b->getDisplay(block);
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
			display = b->getDisplay(block);
			block->setOneFace((int)Faces::Front, display);
			b->setOneFace((int)Faces::Back, display);
			setGenVerticesFlagAt(b->x, b->y, b->z - 1);
		}
	}
	if (checkFace(Up, face))
	{
		block = getBlockAt(b->x, b->y + 1, b->z );
		if (block)
		{
			display = b->getDisplay(block);
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
			display = b->getDisplay(block);
			block->setOneFace((int)Faces::Up, display);
			b->setOneFace((int)Faces::Down, display);
			setGenVerticesFlagAt(b->x, b->y - 1, b->z);
		}
	}
	if (checkFace(Left, face))
	{
		block = getBlockAt(b->x - 1, b->y , b->z);
		if (block)
		{
			display = b->getDisplay(block);
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
			display = b->getDisplay(block);
			block->setOneFace((int)Faces::Left, display);
			b->setOneFace((int)Faces::Right, display);
			setGenVerticesFlagAt(b->x + 1, b->y, b->z);
		}
	}

	setGenVerticesFlagAt(b->x, b->y, b->z);
}
Chunk* Game::getChunkAt(int x, int y, int z)
{
	for (auto c : chunks)
	{
		if (c->x == x && c->y == y && c->z == z)
			return c;
	}
	for (auto c : toAdd)
	{
		if (c->x == x && c->y == y && c->z == z)
			return c;
	}
	return NULL;
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

	while (gameRunning)
	{

		genWorld();



	}
}

void Game::worldDestroyFun()
{

	while (gameRunning)
	{

		desWorld();


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
		if (abs(camPos.x - cPos.x) > range + 1 || abs(camPos.z - cPos.z) > range + 1)
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
		if (abs(camPos.x - cPos.x) > range + 1 || abs(camPos.z - cPos.z) > range + 1)
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

void Game::genMesh()
{
	for (int i = 0; i < 6; i++)
	{
		solidMesh[i]->clearMesh();
		transMesh[i]->clearMesh();
		for (auto c : toDraw)
		{
			solidMesh[i]->addData(c->solidMesh[i]->getData());
			transMesh[i]->addData(c->transMesh[i]->getData());
		}
	}
	for (int i = 6; i < 10; i++)
	{
		transMesh[i]->clearMesh();
		for (auto c : toDraw)
		{
			transMesh[i]->addData(c->transMesh[i]->getData());
		}
	}
}

Game* getCurrentGame()
{
	return Game::game;
}


void addObjectToSave(int x, int y, int z,int ID)
{
	int cX;
	if (x >= 0)
		cX = x / chunkW;
	else
		cX = x / chunkW - 1;
	int cZ;
	if (z >= 0)
		cZ = z / chunkW;
	else
		cZ = z / chunkW - 1;

	Game* g = getCurrentGame();
	if (!g)
		return;
	Chunk* chunk = g->getChunkAt(cX, 0, cZ);
	if (!chunk)
	{

	}
	else
	{

	}
}