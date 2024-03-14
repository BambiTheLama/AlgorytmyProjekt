#include "Chunk.h"
#include "CubeBlock.h"
#include "../scene/Game.h"
#include "FastNoiseLite.h"
#include "../core/VAO.h"
#include "../core/VBO.h"
#include "../core/EBO.h"
Game* Chunk::game = NULL;

Chunk::Chunk(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	for (int j = 0; j < chunkH; j++)
		for (int i = 0; i < chunkW; i++)
			for (int k = 0; k < chunkT; k++)
			{
				blocks[j][i][k] = NULL;
			}
	FastNoiseLite terrain(69);
	terrain.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	terrain.SetFrequency(0.005f);
	terrain.SetFractalType(FastNoiseLite::FractalType_FBm);
	terrain.SetFractalOctaves(3);
	terrain.SetFractalGain(1.5f);
	terrain.SetFractalLacunarity(1.529f);
	terrain.SetFractalWeightedStrength(-0.2f);
	const int height = maxH - minH;

	for (int i = 0; i < chunkW; i++)
		for (int k = 0; k < chunkT; k++)
		{
			int h = (terrain.GetNoise((float)i + this->x * chunkW, (float)k + this->z * chunkT) + 1) / 2 * height + minH - (this->y * chunkH);
			if (h > chunkH)
				h = chunkH;
			for (int j = 0; j < h; j++)
			{
				blocks[j][i][k] = new CubeBlock(i + x * chunkW, j + y * chunkH, k + z * chunkT,0,4,3);
				toAdd.push_back(blocks[j][i][k]);
			}
		}
	vao = new VAO();
	vboVert = new VBO(vertV);
	vboTexture = new VBO(textV);
	index = new EBO(indexV);
	vao->linkData(*vboVert, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
	vao->linkData(*vboTexture, 1, 2, GL_FLOAT, sizeof(glm::vec2), (void*)0);
	index->bind();
	vao->unbind();
	vboVert->unbind();
	vboTexture->unbind();
	index->unbind();
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
		game->setFaceing(b, false);
		
	}

	for (auto b : toDelete)
	{
		game->setFaceing(b->x, b->y, b->z, true);
		delete b;
	}
	if (toAdd.size() > 0 || toDelete.size() > 0)
	{
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
	vao->bind();

	glDrawElements(GL_TRIANGLES, indexV.size(), GL_UNSIGNED_INT, 0);
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
	for (int j = 0; j < chunkH; j++)
		for (int i = 0; i < chunkW; i++)
			for (int k = 0; k < chunkT; k++)
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
	for (int j = 0; j < chunkH; j++)
		for (int i = 0; i < chunkW; i++)
			for (int k = 0; k < chunkT; k++)
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
	for (int j = 0; j < chunkH; j++)
		for (int i = 0; i < chunkW; i++)
			for (int k = 0; k < chunkT; k++)
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