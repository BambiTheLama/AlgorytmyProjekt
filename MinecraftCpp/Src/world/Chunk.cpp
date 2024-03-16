#include "Chunk.h"
#include "CubeBlock.h"
#include "../scene/Game.h"
#include "FastNoiseLite.h"
#include "../core/VAO.h"
#include "../core/VBO.h"
#include "../core/EBO.h"
#include "BlocksCreator.h"
#define forAllBlocks 	for (int j = 0; j < chunkH; j++)\
							for (int i = 0; i < chunkW; i++)\
								for (int k = 0; k < chunkT; k++)
Game* Chunk::game = NULL;

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
	generateTeren();

}

Chunk::~Chunk()
{
	for (int j = 0; j < chunkH; j++)
		for (int i = 0; i < chunkW; i++)
			for (int k = 0; k < chunkT; k++)
				if (blocks[j][i][k])
					delete blocks[j][i][k];
	if (vao)
		delete vao;
	if (index)
		delete index;
	if (vboTexture)
		delete vboTexture;
	if (vboVert)
		delete vboVert;
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
	if (genVertices||toAdd.size() > 0 || toDelete.size() > 0)
	{
		genVertices = false;
		if(!vao)
			vao = new VAO();
		if(!vboVert)
			vboVert = new VBO(vertV);
		if(!vboTexture)
			vboTexture = new VBO(textV);
		if(!index)
			index = new EBO(indexV);
		toDelete.clear();
		toAdd.clear();
		genIndex();
		genVerticesPos();
		genVerticesTexture();
		vao->bind();
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
	vboVert->setNewVertices(vertV);
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
	vboTexture->setNewVertices(textV);
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
	index->setNewVertices(indexV);
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

	for (int i = 0; i < chunkW; i++)
		for (int k = 0; k < chunkT; k++)
		{
			int h = (terrain.GetNoise((float)i + this->x * chunkW, (float)k + this->z * chunkT) + 1) / 2 * height + minH - (this->y * chunkH);
			int h2 = h;
			if (h > chunkH)
				h = chunkH;
			for (int j = 0; j < chunkH && j < h2 - 4; j++)
			{
				blocks[j][i][k] = createBlock(2);
				if (blocks[j][i][k])
				{
					//toAdd.push_back(blocks[j][i][k]);
					blocks[j][i][k]->x = i + x * chunkW;
					blocks[j][i][k]->y = j + y * chunkH;
					blocks[j][i][k]->z = k + z * chunkT;
				}
			}
			for (int j = (h2 - 4) < 0 ? 0 : h2 - 4; j < h; j++)
			{
				blocks[j][i][k] = createBlock(1);
				if (blocks[j][i][k])
				{
					//toAdd.push_back(blocks[j][i][k]);
					blocks[j][i][k]->x = i + x * chunkW;
					blocks[j][i][k]->y = j + y * chunkH;
					blocks[j][i][k]->z = k + z * chunkT;
				}
			}
			if (h >= 0 && h < chunkH)
			{
				if (blocks[h][i][k])
					delete blocks[h][i][k];
				blocks[h][i][k] = createBlock(0);
				if (blocks[h][i][k])
				{
					//toAdd.push_back(blocks[h][i][k]);
					blocks[h][i][k]->x = i + x * chunkW;
					blocks[h][i][k]->y = h + y * chunkH;
					blocks[h][i][k]->z = k + z * chunkT;
				}



			}

		}
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