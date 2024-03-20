#include "Chunk.h"
#include "CubeBlock.h"
#include "../scene/Game.h"
#include "FastNoiseLite.h"
#include "../core/VAO.h"
#include "../core/VBO.h"
#include "../core/EBO.h"
#include "BlocksCreator.h"
#include <json.hpp>
#include <fstream>
#include "../Properties.h"
#include <direct.h>

#define forAllBlocks 	for (int j = 0; j < chunkH; j++)\
							for (int i = 0; i < chunkW; i++)\
								for (int k = 0; k < chunkT; k++)
Game* Chunk::game = NULL;
std::string Chunk::path = "World/";

Chunk::Chunk(int x, int y, int z)
{

	vertV = std::vector<glm::vec3>();
	textV = std::vector<glm::vec2>();
	indexV = std::vector<GLuint>();
	this->x = x;
	this->y = y;
	this->z = z;
	forAllBlocks
	{
		blocks[j][i][k] = NULL;
	}
	if(!loadGame())
		generateTeren();
	setFaceing();
	genIndex();
	genVerticesPos();
	genVerticesTexture();

}

Chunk::~Chunk()
{

	if (vao)
		delete vao;
	if (index)
		delete index;
	if (vboTexture)
		delete vboTexture;
	if (vboVert)
		delete vboVert;
	if (wasCleared)
		return;
	forAllBlocks
	if (blocks[j][i][k])
	{
		delete blocks[j][i][k];
	}
}

void Chunk::start()
{
	if (!vao)
		vao = new VAO();
	if (!vboVert)
		vboVert = new VBO(vertV);
	if (!vboTexture)
		vboTexture = new VBO(textV);
	if (!index)
		index = new EBO(indexV);
	vao->bind();
	index->setNewVertices(indexV);
	vboTexture->setNewVertices(textV);
	vboVert->setNewVertices(vertV);
	vao->linkData(*vboVert, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
	vao->linkData(*vboTexture, 1, 2, GL_FLOAT, sizeof(glm::vec2), (void*)0);
	index->bind();
	vao->unbind();
}

void Chunk::update(float deltaTime)
{
	for (auto b : toAdd)
	{
		if (b)
		{
			if (b->faceToSetUp() <= 0)
				continue;
			game->setFaceing(b, false, b->faceToSetUp());
		}
	}

	for (auto b : toDelete)
	{
		if (b)
		{
			game->setFaceing(b->x, b->y, b->z, true);
			delete b;
		}

	}
	if (genVertices || toAdd.size() > 0 || toDelete.size() > 0)
	{
		genVertices = false;
		toDelete.clear();
		toAdd.clear();
		std::thread t1 = std::thread(&Chunk::genIndex, this);
		std::thread t2 = std::thread(&Chunk::genVerticesPos, this);
		std::thread t3 = std::thread(&Chunk::genVerticesTexture, this);

		t1.join();
		t2.join();
		t3.join();
		genIndex();
		genVerticesPos();
		genVerticesTexture();
		vao->bind();
		index->setNewVertices(indexV);
		vboTexture->setNewVertices(textV);
		vboVert->setNewVertices(vertV);
		vao->linkData(*vboVert, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
		vao->linkData(*vboTexture, 1, 2, GL_FLOAT, sizeof(glm::vec2), (void*)0);
		index->bind();
		vao->unbind();

	}


}

void Chunk::draw()
{
	if (indexV.size() > 0)
	{
		vao->bind();
		glDrawElements(GL_TRIANGLES, indexV.size(), GL_UNSIGNED_INT, 0);
	}

}

Block* Chunk::getBlock(int x, int y, int z)
{
	x -= this->x * chunkW;
	y -= this->y * chunkH;
	z -= this->z * chunkT;
	if (x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT)
		return blocks[y][x][z];
	return NULL;
}

void Chunk::deleteBlock(Block* b)
{
	int x = b->x - this->x * chunkW;
	int y = b->y - this->y * chunkH;
	int z = b->z - this->z * chunkT;
	if (x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT)
	{
		toDelete.push_back(blocks[y][x][z]);
		blocks[y][x][z] = NULL;
	}
}

bool Chunk::isThisChunk(int x, int y, int z)
{
	x -= this->x * chunkW;
	y -= this->y * chunkH;
	z -= this->z * chunkT;
	return x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT;
}

void Chunk::save()
{
	struct stat sb;
	if (stat(path.c_str(), &sb) != 0)
		_mkdir(path.c_str());
	int times = 0;
	int ID = 0;
	int b = 0;
	std::string pathFile = path + "chunk " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ".json";
	std::ofstream save(pathFile);
	if (!save.is_open())
	{
#ifdef DebugFailMode
		printf("[Fail]: Cant Create file [%s]\n", pathFile.c_str());
#endif
		return;
	}

	nlohmann::json json;

	forAllBlocks
	{
		int IdNow = -1;
		if (blocks[j][i][k])
			IdNow = blocks[j][i][k]->getID();

		if (IdNow != ID)
		{
			if (times > 0)
			{
				json["Blocks"][b][0] = ID;
				json["Blocks"][b][1] = times;
				b++;
			}
			ID = IdNow;
			times = 1;
			continue;
		}
		else
		{
			times++;
		}
		
	}
	json["Blocks"][b][0] = ID;
	json["Blocks"][b][1] = times;

	save << json;
	save.close();

}

bool Chunk::loadGame()
{
	int times = 0;
	int ID = 0;
	int b = 0;
	std::string pathFile = path + "chunk " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ".json";
	std::ifstream read(pathFile);
	if (!read.is_open())
	{
		read.close();
		return false;
	}

	nlohmann::json json;
	read >> json;
	read.close();
	if (!json.contains("Blocks") || json["Blocks"].size() <= 0)
		return false;
	forAllBlocks
	{
		if (times <= 0)
		{
			if (json["Blocks"].size() <= b)
				return true;
			ID = json["Blocks"][b][0];
			times = json["Blocks"][b][1];
			b++;
		}
		times--;
		if (ID < 0)
		{
			continue;
		}
		else
		{
			blocks[j][i][k] = createBlock(ID, i + x * chunkW, j + y * chunkH, k + z * chunkT);
		}



	}

	return true;
}

void Chunk::clearBlocks()
{
	forAllBlocks
	if (blocks[j][i][k])
	{
		delete blocks[j][i][k];
		blocks[j][i][k] = NULL;
	}
	wasCleared = true;
}

void Chunk::genVerticesPos()
{
	vertV.clear();
	GLuint lastIndex = 0;
	forAllBlocks
	if (blocks[j][i][k])
	{
		if (blocks[j][i][k]->indexSize() <= 0)
			continue;
		std::vector<glm::vec3> vertTmp = blocks[j][i][k]->getVertexPos();

		vertV.insert(vertV.end(), vertTmp.begin(), vertTmp.end());
	}

}

void Chunk::genVerticesTexture()
{
	textV.clear();
	forAllBlocks
	if (blocks[j][i][k])
	{
		if (blocks[j][i][k]->indexSize() <= 0)
			continue;
		std::vector<glm::vec2> textTmp = blocks[j][i][k]->getVertexTexture();

		textV.insert(textV.end(), textTmp.begin(), textTmp.end());
	}
}

void Chunk::genIndex()
{
	indexV.clear();
	GLuint lastIndex = 0;
	forAllBlocks
	if (blocks[j][i][k])
	{
		if (blocks[j][i][k]->indexSize() <= 0)
			continue;
		std::vector<GLuint> indexTmp = blocks[j][i][k]->getIndex();
		for (auto ind : indexTmp)
		{
			indexV.push_back(ind + lastIndex);
		}
		lastIndex += blocks[j][i][k]->indexSize();
	}

}

void Chunk::generateTeren()
{
	FastNoiseLite terrain(2137);
	terrain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	terrain.SetFrequency(0.003f);
	terrain.SetFractalType(FastNoiseLite::FractalType_FBm);
	terrain.SetFractalOctaves(2);
	terrain.SetFractalGain(5.0f);
	terrain.SetFractalLacunarity(1.529f);
	terrain.SetFractalWeightedStrength(3.304f);
	const int height = maxH - minH;
	const int dirtSize = 5;
	for (int i = 0; i < chunkW; i++)
		for (int k = 0; k < chunkT; k++)
		{
			int h = (terrain.GetNoise((float)i + this->x * chunkW, (float)k + this->z * chunkT) + 1) / 2 * height + minH - (this->y * chunkH);
			if (h <= 2)
				h = 2;
			for (int j = 0; j < chunkH && j < h - dirtSize; j++)
			{
				if (blocks[j][i][k])
					delete blocks[j][i][k];
				blocks[j][i][k] = createBlock(2, i + x * chunkW, j + y * chunkH, k + z * chunkT);
			}
			for (int j = (h - dirtSize) < 0 ? 0 : h - dirtSize; j < h && j < chunkH; j++)
			{
				blocks[j][i][k] = createBlock(j == h - 1 ? 0 : 1,  i + x * chunkW, j + y * chunkH, k + z * chunkT);

			}


		}
	
}

void Chunk::setFaceing()
{
	forAllBlocks
	{
		if (j - 1 > 0 && blocks[j][i][k] && blocks[j - 1][i][k] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j - 1][i][k]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Down, false);
			blocks[j - 1][i][k]->setOneFace((int)Faces::Up, false);
		}
		if (j + 1 < chunkH && blocks[j][i][k] && blocks[j + 1][i][k] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j + 1][i][k]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Up, false);
			blocks[j + 1][i][k]->setOneFace((int)Faces::Down, false);
		}
		if (i - 1 > 0 && blocks[j][i][k] && blocks[j][i - 1][k] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j][i - 1][k]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Left, false);
			blocks[j][i - 1][k]->setOneFace((int)Faces::Right, false);
		}
		if (i + 1 < chunkW && blocks[j][i][k] && blocks[j][i + 1][k] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j][i + 1][k]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Right, false);
			blocks[j][i + 1][k]->setOneFace((int)Faces::Left, false);
		}
		if (k - 1 > 0 && blocks[j][i][k] && blocks[j][i][k - 1] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j][i][k - 1]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Back, false);
			blocks[j][i][k - 1]->setOneFace((int)Faces::Front, false);
		}
		if (k + 1 < chunkT && blocks[j][i][k] && blocks[j][i][k + 1] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j][i][k + 1]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Front, false);
			blocks[j][i][k + 1]->setOneFace((int)Faces::Back, false);
		}
		if ((i == 0 || i == chunkW - 1 || j == 0 || j == chunkH - 1 || k == 0 || k == chunkT - 1) && blocks[j][i][k])
		{
			toAdd.push_back(blocks[j][i][k]);
		}
	}
}