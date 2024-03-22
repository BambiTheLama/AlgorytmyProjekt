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
#include "../core/Engine.h"
#include "../core/Shader.h"
#include <glm/gtc/matrix_transform.hpp>

#define forAllBlocks 	for (int j = 0; j < chunkH; j++)\
							for (int i = 0; i < chunkW; i++)\
								for (int k = 0; k < chunkT; k++)
Game* Chunk::game = NULL;
std::string Chunk::path = "World/";

Chunk::Chunk(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	forAllBlocks
	{
		blocks[j][i][k] = NULL;
	}
	if(!loadGame())
		generateTeren();

	genVerticesPos();
	setFaceing();
	indexV = std::vector<GLuint>();

}

Chunk::~Chunk()
{

	if (vao)
		delete vao;
	if (ebo)
		delete ebo;
	if (vbo)
		delete vbo;
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
	if (!vbo)
		vbo = new VBO();

	if (!ebo)
		ebo = new EBO(indexV);

	
	verticesV.clear();
	verticesT.clear();

	vao->linkData(*vbo, 0, 1, GL_FLOAT, sizeof(float), (void*)0);
	vao->bind();
	ebo->bind();
	vao->unbind();
	ebo->unbind();
}

void Chunk::update(float deltaTime)
{
	for (auto b : toAdd)
	{

		if (b->faceToSetUp() <= 0)
			continue;
		game->setFaceing(b, x * chunkW, y * chunkH, z * chunkT, b->isTransparent(), b->faceToSetUp());

	}

	for (auto b : toDelete)
	{
		game->setFaceing(b->x + x * chunkW, b->y + y * chunkH, b->z + z * chunkT, true);
		delete b;
	}
	if (genVertices || toAdd.size() > 0 || toDelete.size() > 0)
	{
		genVertices = false;
		toDelete.clear();
		toAdd.clear();
		genVerticesPos();
		vao->bind();
		ebo->setNewVertices(indexV);
		ebo->bind();
		vbo->setNewVertices(vertices);
		verticesV.clear();
		verticesT.clear();

		vao->linkData(*vbo, 0, 1, GL_FLOAT, sizeof(GLuint), (void*)0);

		vao->unbind();


	}


}

void Chunk::draw(Shader* s)
{
	if (indexV.size() > 0)
	{
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(x * chunkW, y * chunkH, z * chunkT));
		s->setUniformMat4(model, "model");
		vao->bind();
		ebo->bind();
		glDrawElements(GL_TRIANGLES, indexV.size(), GL_UNSIGNED_INT, 0);
		vao->unbind();
		ebo->unbind();
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
			blocks[j][i][k] = createBlock(ID, i, j, k);
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
	indexV.clear();
	vertices.clear();
	GLuint lastIndex = 0;
	forAllBlocks
	if (blocks[j][i][k])
	{
		if (blocks[j][i][k]->indexSize() <= 0)
			continue;

		std::vector<GLuint> indexTmp = blocks[j][i][k]->getIndex();

		std::vector<GLuint> vertTmp = blocks[j][i][k]->getVertex();

		for (auto ind : indexTmp)
		{
			indexV.push_back(ind + lastIndex);
		}

		lastIndex += blocks[j][i][k]->indexSize();

		vertices.insert(vertices.end(), vertTmp.begin(), vertTmp.end());

	}

}

void Chunk::generateTeren()
{
	FastNoiseLite terrain(666);
	terrain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	terrain.SetFrequency(0.004f);
	terrain.SetFractalType(FastNoiseLite::FractalType_FBm);
	terrain.SetFractalOctaves(3);
	terrain.SetFractalGain(0.3f);
	terrain.SetFractalLacunarity(1.529f);
	terrain.SetFractalWeightedStrength(3.304f);

	FastNoiseLite river(2137);
	river.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	river.SetFrequency(0.001f);
	river.SetFractalType(FastNoiseLite::FractalType_PingPong);
	river.SetFractalOctaves(3);
	river.SetFractalGain(0.11f);
	river.SetFractalLacunarity(1.72f);
	river.SetFractalWeightedStrength(2.02f);
	river.SetFractalPingPongStrength(2.44f);
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
				blocks[j][i][k] = createBlock(2, i, j, k);
			}
			int dirtStart = h - dirtSize;
			if (dirtStart < 0)
				dirtStart = 0;
			int endPos = h;
			if (endPos < waterH)
				endPos--;

			for (int j = dirtStart; j < endPos && j < chunkH; j++)
			{
				if (blocks[j][i][k])
					delete blocks[j][i][k];
				blocks[j][i][k] = createBlock(j == h - 1 ? 0 : 1, i, j, k);

			}
			for (int j = endPos; j < endPos + 3 && j <= waterH && j < chunkH; j++)
			{
				if (blocks[j][i][k])
					delete blocks[j][i][k];
				blocks[j][i][k] = createBlock(4, i, j, k);

			}
			for (int j = endPos + 3; j <= waterH && j < chunkH; j++)
			{
				if (blocks[j][i][k])
					delete blocks[j][i][k];
				blocks[j][i][k] = createBlock(11, i, j, k);

			}

		}
	
}

void Chunk::setFaceing()
{
	forAllBlocks
	{
		if (!blocks[j][i][k])
			continue;
		const bool isTransparent = blocks[j][i][k]->isTransparent();
		if (j - 1 > 0  && blocks[j - 1][i][k] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j - 1][i][k]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Down, isTransparent);
			blocks[j - 1][i][k]->setOneFace((int)Faces::Up, isTransparent);
		}
		if (j + 1 < chunkH  && blocks[j + 1][i][k] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j + 1][i][k]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Up, isTransparent);
			blocks[j + 1][i][k]->setOneFace((int)Faces::Down, isTransparent);
		}
		if (i - 1 > 0  && blocks[j][i - 1][k] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j][i - 1][k]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Left, isTransparent);
			blocks[j][i - 1][k]->setOneFace((int)Faces::Right, isTransparent);
		}
		if (i + 1 < chunkW  && blocks[j][i + 1][k] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j][i + 1][k]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Right, isTransparent);
			blocks[j][i + 1][k]->setOneFace((int)Faces::Left, isTransparent);
		}
		if (k - 1 > 0  && blocks[j][i][k - 1] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j][i][k - 1]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Back, isTransparent);
			blocks[j][i][k - 1]->setOneFace((int)Faces::Front, isTransparent);
		}
		if (k + 1 < chunkT  && blocks[j][i][k + 1] &&
			!blocks[j][i][k]->isTransparent() && !blocks[j][i][k + 1]->isTransparent())
		{
			blocks[j][i][k]->setOneFace((int)Faces::Front, isTransparent);
			blocks[j][i][k + 1]->setOneFace((int)Faces::Back, isTransparent);
		}
		if ((i == 0 || i == chunkW - 1 || j == 0 || j == chunkH - 1 || k == 0 || k == chunkT - 1))
		{
			if (blocks[j][i][k])
				toAdd.push_back(blocks[j][i][k]);
		}
	}
}