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
#include <math.h>


#define noSave
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
		if (!b)
			continue;
		blocks[b->y][b->x][b->z] = NULL;
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

void Chunk::deleteBlock(int x, int y, int z)
{
	x -= this->x * chunkW;
	y -= this->y * chunkH;
	z -= this->z * chunkT;
	if (x >= 0 && x < chunkW && y >= 0 && y < chunkH && z >= 0 && z < chunkT)
	{
		for (auto d : toDelete)
			if (d == blocks[y][x][z])
				return;
		toDelete.push_back(blocks[y][x][z]);
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
#ifdef noSave
	return;
#endif // noSave

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
float getValue(float v)
{
	if (v <= -1.0f)
		return -log(-v) / 6.0f - 1;
	if (v >= 1.0f)
		return log(v) / 6.0f + 1;
	return powf(v, 3);
}




void genValues(float** tab)
{
	int w = chunkW - 1;
	int t = chunkT - 1;
	for (int x = 1; x < w; x++)
	{
		float v = (float)x / (float)chunkW;
		tab[x][0] = (1.0f - v) * tab[0][0] + v * tab[w][0];
		tab[x][t] = (1.0f - v) * tab[0][t] + v * tab[w][t];

	}
	for (int z = 1; z < t; z++)
	{
		float v = (float)z / (float)chunkT;
		tab[0][z] = (1.0f - v) * tab[0][0] + v * tab[0][t];
		tab[w][z] = (1.0f - v) * tab[w][0] + v * tab[w][t];
	}
	for (int x = 1; x < w; x++)
		for (int z = 1; z < t; z++)
		{
			float vx = (float)x / (float)chunkW;
			float vz = (float)z / (float)chunkT;
			tab[x][z] = ((1.0f - vx) * tab[0][z] + vx * tab[w][z] + (1.0f - vz) * tab[x][0] + vz * tab[x][t]) / 2;
		}
}
#define NoiseSizeW 500
#define NoiseSizeH 500
static float noise2D[NoiseSizeH][NoiseSizeW];

void setNoiseSeed(int seed)
{
	float noise2DTMP[NoiseSizeH][NoiseSizeW];
	srand(seed);

	for (int x = 0; x < NoiseSizeW; x++)
		for (int y = 0; y < NoiseSizeH; y++)
		{
			noise2DTMP[y][x] = ((float)rand() / RAND_MAX);
		}
	int octaves = 5;
	int bias = 2;
	for (int x = 0; x < NoiseSizeW; x++)
		for (int y = 0; y < NoiseSizeH; y++)
		{
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < octaves; o++)
			{
				int nPitch = NoiseSizeW >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % NoiseSizeW;
				int nSampleY2 = (nSampleY1 + nPitch) % NoiseSizeW;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * noise2DTMP[nSampleY1][nSampleX1] + fBlendX * noise2DTMP[nSampleY1][nSampleX2];
				float fSampleB = (1.0f - fBlendX) * noise2DTMP[nSampleY2][nSampleX1] + fBlendX * noise2DTMP[nSampleY2][nSampleX2];

				fScaleAcc += fScale;
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / bias;
			}

			// Scale to seed range
			noise2D[y][x] = fNoise / fScaleAcc;
		}
}
const float frq = 1.0f;

float getValueTerein(int x, int z)
{
	int Six = 0;
	int Siz = 0;
	int Eix = 0;
	int Eiz = 0;
	float v = 0.0f;

	Six = (int)(frq * abs(x)) % NoiseSizeW;
	Siz = (int)(frq * abs(z)) % NoiseSizeH;
	Eix = (Six + 1) % NoiseSizeW;
	Eiz = (Siz + 1) % NoiseSizeH;

	float xp = frq * abs(-x);
	xp -= (int)xp;
	float zp = frq * abs(-z);
	zp -= (int)zp;

	float vd = noise2D[Siz][Six] * (1.0f - xp) + noise2D[Siz][Eix] * xp;
	float vu = noise2D[Eiz][Six] * (1.0f - xp) + noise2D[Eiz][Eix] * xp;
	v = vd * (1.0f - zp) + vu * zp;
	return (v + 1) / 2;
}

void Chunk::generateTeren()
{
	float **tab = new float*[chunkW];	
	for (int i = 0; i < chunkW; i++)
		tab[i] = new float[chunkT];
	int w = this->x * chunkW;
	int t = this->z * chunkT;
	tab[0][0] = getValueTerein(x, z);
	tab[chunkW - 1][0] = getValueTerein(x + 1, z);
	tab[chunkW - 1][chunkT - 1] = getValueTerein(x + 1, z + 1);
	tab[0][chunkT - 1] = getValueTerein(x, z + 1);
	genValues(tab);
	const int height = maxH - minH;
	const int dirtSize = 5;
	for (int i = 0; i < chunkW; i++)
		for (int k = 0; k < chunkT; k++)
		{
			float x = i + this->x * chunkW;
			float z = k + this->z * chunkT;

			float tereinV = getValueTerein(x, z);
			int h = minH + tereinV * height;
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

	for (int i = 0; i < chunkW; i++)
		delete tab[i];
	delete tab;
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