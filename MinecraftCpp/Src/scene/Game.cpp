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
#include <windows.h> 

Game* Game::game = NULL;

Game::Game(int w,int h,GLFWwindow* window, ImGuiIO* io)
{
	this->io = io;
	Chunk::game = this;
	cube = new Cube();

	this->window = window;

	vao = new VAO();
	vbo = new VBO();
	ebo = new EBO();
	selection = new Texture("Res/Selected.jpg");


	shader = new Shader("Shader/Diff.vert", "Shader/Diff.frag");
	shaderShadow = new Shader("Shader/Diff.vert", "Shader/Shadow.frag");

	Blocks = new GameTextures("Res/Blocks/", "Textures");
	BlocksH = new GameTextures("Res/BlocksH/", "Textures");
	BlocksN = new GameTextures("Res/BlocksN/", "Textures");
	waterTexture = new Texture("Res/Water/Water.png");
	waterTexture2 = new Texture("Res/Water/Water_C.png");
	skybox = new SkyBox("Res/Skybox/");

	shader->active();
	Blocks->setTextures(*shader, "tex0");
	BlocksH->setTextures(*shader, "texH");
	BlocksN->setTextures(*shader, "texN");

	glm::mat4 modelMat = glm::mat4(1.0f);
	shader->setUniformMat4(modelMat, "model");
	shader->setUniformVec4(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "modelColor");
	ShadowMap = new RenderTexture(2048, 2048);
	ShadowMap->use(*shader, "texShadow");
	camera = new Camera(w, h, 0.1f, 1000, 60, glm::vec3(0.0f, 100.0f, -1.0f));
	game = this;
}

Game::~Game()
{
	delete skybox;
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

	delete shader;
	delete Blocks;
	delete BlocksH;
	delete BlocksN;
	delete waterTexture;
	delete waterTexture2;

	delete shaderShadow;
	game = NULL;
	Chunk::saveBlockData();
}

void Game::start()
{
	gameRunning = true;
	if(!worldGenerateT.joinable())
		worldGenerateT = std::thread(&Game::worldGenerateFun, this);
	if (!worldDestroyT.joinable())
		worldDestroyT = std::thread(&Game::worldDestroyFun, this);
}

glm::vec3 camPos;

static bool compareObj(Chunk* c1, Chunk* c2)
{
	return glm::distance(c1->getPos(), camPos) > glm::distance(c2->getPos(), camPos);
}

void Game::update(float deltaTime)
{
	time += deltaTime;
	if(!io->WantCaptureMouse)
		camera->update(window, deltaTime);
	chunksMutex.lock();

		//toAddMutex.lock();
		for (auto c : chunks)
			c->update(deltaTime);


	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		debug = false;
	}
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		debug = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
	{
		gameRunning = false;

	}
	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
	{
		start();
	}

	glm::vec3 pos = camera->getPos();
	glm::vec3 dir = camera->getDir();


	if (toAddMutex.try_lock())
	{
		for (auto a : toAdd)
		{
			a->start();
			a->update(0.0f);

			auto it = std::find(chunks.begin(), chunks.end(), a);
			if (it == chunks.end())
				chunks.push_back(a);
		}
		toAdd.clear();
		toAddMutex.unlock();
	}
	if (toDeleteMutex.try_lock())
	{
		for (auto d : toDelete)
		{
			auto it = std::find(chunks.begin(), chunks.end(), d);
			if (it != chunks.end())
				chunks.erase(it);

			reloadChunksNextTo(d);

			delete d;
		}

		toDelete.clear();
		toDeleteMutex.unlock();
	}


	toDraw = chunks;

	camPos = camera->getPos();
	std::sort(toDraw.begin(), toDraw.end(), compareObj);
	if (glfwGetKey(window, GLFW_KEY_TAB))
		for (auto c : chunks)
		{
			c->setFacing();
			c->genVerticesFlag();
		}

	findLookingAtBLock();
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) && b)
	{
		deleteBlock(blockPos.x, blockPos.y, blockPos.z);
	}

	chunksMutex.unlock();
}
#include "../world/Blocks/BlocksCreator.h"
#include "../world/Blocks/WaveColapseFunction.h"

void Game::draw()
{

	bool spawn = false;
	bool genVillage = false;
	static int seed = Chunk::seed;
	static int rotate = 0;
	static int rangeVillage = 8;
	static int posToSpawn[3] = { 68,waterH+1,-120 };
	static int blockID = 0;
	ImGui::Begin(" ");
	if (ImGui::CollapsingHeader("Render"))
	{
		ImGui::DragInt("ChunkRange", &range, 1, 1, 12);
		ImGui::Checkbox("StopGenDestyWorld", &stopGenDestyWorld);
	}

	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::DragFloat3("LightDir", lightDir, 0.01, -1, 1);
		ImGui::ColorEdit3("LightColor", lightColor);
	}
	if (ImGui::CollapsingHeader("Spawn Objects"))
	{
		ImGui::Checkbox("Spawn Block", &spawn);
		ImGui::DragInt3("Pos to Spawn", posToSpawn);
		ImGui::DragInt("Block Id", &blockID, 1);
		ImGui::DragInt("Rotate", &rotate, 1, 0, 3);
		ImGui::Checkbox("Gen Village", &genVillage);
		ImGui::DragInt("Village range", &rangeVillage, 1, 0, 20);
	}

	ImGui::Text("Camera Pos [x,y,z] %lf %lf %lf", cameraPos.x, cameraPos.y, cameraPos.z);

	ImGui::End();


	if (spawn)
	{
		Block* b = createBlock(blockID, posToSpawn[0], posToSpawn[1], posToSpawn[2]);
		StructureHandler* s = dynamic_cast<StructureHandler*>(b);
		if (s)
		{
			for (int i = 0; i < rotate; i++)
				s->rotate();
		}

		deleteBlock(posToSpawn[0], posToSpawn[1], posToSpawn[2]);
		if (!addBlock(b))
			delete b;
	}
	if (genVillage)
	{
		Tile** t = generateVillage(rangeVillage);
		for (int x = 0; x < rangeVillage; x++)
			for (int z = 0; z < rangeVillage; z++)
			{
				if (t[x + z * rangeVillage])
				{
					Tile* tile = t[x + z * rangeVillage];
					int posX = posToSpawn[0] + x * StructureTileSize - rangeVillage / 2 * StructureTileSize;
					int posY = posToSpawn[1];
					int posZ = posToSpawn[2] + z * StructureTileSize - rangeVillage / 2 * StructureTileSize;

					StructureHandler* str = createStructure(tile->ID, posX, posY, posZ);
					if (!str)
						continue;
					for (int i = 0; i < (tile->rotate)%4; i++)
						str->rotate();
					if (!addBlock(str))
						delete str;
				}
			}

		for (auto i = 0; i < rangeVillage * rangeVillage; i++)
			if (t[i])
				delete t[i];
		delete t;
	}

	skybox->draw(camera);

	camera->useCamera(*shader, "camera");

	glm::mat4 model(1.0f);
	shader->setUniformMat4(model, "model");

	ShadowMap->startUse();

	cameraDir = camera->getDir();
	cameraPos = camera->getPos();
	glm::vec3 lightDir = glm::vec3(this->lightDir[0], this->lightDir[1], this->lightDir[2]);
	glm::vec3 shadowMapLightDir = -lightDir;
	camera->setDir(shadowMapLightDir);

	camera->setUseProjection(false);
	camera->newPos(glm::vec3(((int)cameraPos.x / 16) * 16, 255, ((int)cameraPos.z / 16) * 16));

	shaderShadow->active();
	camera->useCamera(*shaderShadow, "camera");
	renderScene(shaderShadow, true);
	ShadowMap->endUse();


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader->active();
	waterTexture->useTexture(*shader, "waterTex0");
	waterTexture->bind();
	waterTexture2->useTexture(*shader, "waterTex1");
	waterTexture2->bind();
	shader->setUniformI1(debug, "debug");
	glm::mat4 modelMat(1.0f);
	shader->setUniformMat4(modelMat, "model");
	shader->setUniformVec3(camera->getPos(), "camPos");
	shader->setUniformVec3(glm::vec3(lightColor[0], lightColor[1], lightColor[2]), "lightColor");
	camera->useCamera(*shader, "lightProjection");
	shader->setUniformVec3(lightDir, "lightDir");
	shader->setUniformVec3(shadowMapLightDir, "shadowMapLightDir");

	ShadowMap->use(*shader, "texShadow");

	camera->setDir(cameraDir);
	camera->newPos(cameraPos);
	camera->setUseProjection(true);
	camera->useCamera(*shader, "camera");
	renderScene(shader,true);

	drawBlock();

	ShadowMap->draw();

}

void Game::renderScene(Shader* s,bool trans)
{
	chunksMutex.lock();
	glEnable(GL_DEPTH_TEST);
	s->setUniformVec4(glm::vec4(1, 1, 1, 1), "modelColor");
	s->setUniformF1(time, "time");
	Blocks->bindTextures();
	BlocksH->bindTextures();
	BlocksN->bindTextures();

	for (auto c : toDraw)
	{
		c->draw(s);
	}
	glDisable(GL_DEPTH_TEST);
	chunksMutex.unlock();
}

void Game::drawBlock()
{
	if (!b)
		return;
	drawCubeAt(blockPos.x, blockPos.y, blockPos.z, camera,b->getFaces());

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
		{
			c->deleteBlock(x, y, z);
			return;
		}

	for (auto c : toAdd)
		if (c->isThisChunk(x, y, z))
		{
			c->deleteBlock(x, y, z);
			return;
		}
}

bool Game::isBlockAt(int x, int y, int z)
{
	for (auto c : chunks)
		if (c->isThisChunk(x, y, z))
			return c->isBlockAt(x, y, z);
	return false;
}

bool Game::addBlock(Block* b)
{
	if (!b)
		return false;
	int x = b->x;
	int y = b->y;
	int z = b->z;
	for (auto c : chunks)
		if (c->isThisChunk(x, y, z))
			return c->addBlock(b);
	for (auto a : toAdd)
		if (a->isThisChunk(x, y, z))
			return a->addBlock(b);
	if (b->ID < 0)
	{
		StructureHandler* heandler = dynamic_cast<StructureHandler*>(b);
		if (heandler)
			Chunk::saveBlockToChunk(x, y, z, heandler->ID, heandler->getTimeRotated(), heandler->getVariant());
	}
	else
	{
		Chunk::saveBlockToChunk(x, y, z, b->ID);
	}

	return false;
}

void Game::setFacing(int x, int y, int z, bool display, char face)
{
#define setFacingDef(CheckingFace,x1,y1,z1) \
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
	setFacingDef(Front, x, y, z - 1)
	setFacingDef(Back, x, y, z + 1)
	setFacingDef(Left, x + 1, y, z)
	setFacingDef(Right, x - 1, y, z)
	setFacingDef(Up, x, y - 1, z)
	setFacingDef(Down, x, y + 1, z)

	setGenVerticesFlagAt(x, y, z);
}

void Game::setFacing(Block* b, char face)
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

void Game::findLookingAtBLock()
{
	glm::vec3 cameraPos = camera->getPos();
	glm::vec3 cameraDir = camera->getDir();
	int i;
	for (i = 0; i < 100; i++)
	{
		glm::vec3 pos = glm::vec3(cameraPos.x + cameraDir.x * i, cameraPos.y + cameraDir.y * i, cameraPos.z + cameraDir.z * i);
		b = getBlockAt(pos.x, pos.y, pos.z);

		
		if (b)
			break;
	}
	if (!b)
		return;

	blockPos = cameraPos + glm::vec3(cameraDir.x * i, cameraDir.y * i, cameraDir.z * i);
}

void Game::reloadChunksNextTo(Chunk* c)
{
	glm::vec3 pos = c->getLocation();
	Chunk* chunkL = getChunkAt(pos.x - 1, pos.y, pos.z);
	if (chunkL)
		chunkL->reloadBlocksRight();
	Chunk* chunkR = getChunkAt(pos.x + 1, pos.y, pos.z);
	if (chunkR)
		chunkR->reloadBlocksLeft();
	Chunk* chunkF = getChunkAt(pos.x, pos.y, pos.z + 1);
	if (chunkF)
		chunkF->reloadBlocksBack();
	Chunk* chunkB = getChunkAt(pos.x, pos.y, pos.z - 1);
	if (chunkB)
		chunkB->reloadBlocksFront();
}

void Game::clearChunks()
{
	for (auto c : toAdd)
		delete c;
	for (auto c : chunks)
	{
		delete c;
	}
	chunks.clear();
	toAdd.clear();
	toSave.clear();
	toDelete.clear();
	toDraw.clear();
}

void Game::worldGenerateFun()
{
	while (gameRunning)
	{
		if (!stopGenDestyWorld)
			genWorld();
		else
			Sleep(1);
	}
}

void Game::worldDestroyFun()
{
	while (gameRunning)
	{
		if (!stopGenDestyWorld)
			desWorld();
		else
			Sleep(1);
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

	std::vector<std::thread> chunksToGen;
	while (posToGenChunk.size() > 0)
	{
		glm::vec2 pos = posToGenChunk.back();
		posToGenChunk.pop_back();
		chunksToGen.push_back(std::thread(&Game::genOneChunk, this, pos, camPos));
	}
	for (int i = 0; i < chunksToGen.size(); i++)
		chunksToGen[i].join();

}

void Game::genOneChunk(glm::vec2 pos,glm::vec3 camPos)
{
	if (abs(pos.x - camPos.x) > range || abs(pos.y - camPos.z) > range)
		return;
	
	Chunk* c = new Chunk(pos.x, 0, pos.y);

	toAddMutex.lock();
	if (c)
		toAdd.push_back(c);
	toAddMutex.unlock();
}

void Game::desWorld()
{
	glm::vec3 camPos = camera->getPos();
	camPos.x /= chunkW;
	camPos.y /= chunkH;
	camPos.z /= chunkT;
	int range = this->range + 1;
	chunksMutex.lock();
	for (auto c : chunks)
	{
		if (abs(camPos.x - c->x) > range || abs(camPos.z - c->z) > range)
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
		if (abs(camPos.x - c->x) > range || abs(camPos.z - c->z) > range)
		{
			auto it = std::find(toDelete.begin(), toDelete.end(), c);
			if (it == toDelete.end())
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
}