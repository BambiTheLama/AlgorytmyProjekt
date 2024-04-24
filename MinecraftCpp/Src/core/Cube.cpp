#include "Cube.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include "Engine.h"
#include <math.h>

Cube::Cube()
{
	face = 0b111111;
}

void Cube::setFacing(char faces)
{
	this->face = faces;
}

void Cube::setOneFace(char face, bool state)
{
	char f = (char)this->face & ~((char)face);
	this->face = (f | ((char)face * state));
}

std::vector<glm::vec3> Cube::getVertexPos() const
{
	std::vector<glm::vec3> vertexPos;
	if (face <= 0)
		return vertexPos;
	if (checkFace(Front, face))
	{
		vertexPos.push_back(glm::vec3(1, 1, 1));
		vertexPos.push_back(glm::vec3(1, 0, 1));
		vertexPos.push_back(glm::vec3(0, 1, 1));
		vertexPos.push_back(glm::vec3(0, 0, 1));
	}
	if (checkFace(Back, face))
	{
		vertexPos.push_back(glm::vec3(1, 1, 0));
		vertexPos.push_back(glm::vec3(1, 0, 0));
		vertexPos.push_back(glm::vec3(0, 1, 0));
		vertexPos.push_back(glm::vec3(0, 0, 0));
	}
	if (checkFace(Left, face))
	{
		vertexPos.push_back(glm::vec3(0, 1, 1));
		vertexPos.push_back(glm::vec3(0, 0, 1));
		vertexPos.push_back(glm::vec3(0, 1, 0));
		vertexPos.push_back(glm::vec3(0, 0, 0));
	}
	if (checkFace(Right, face))
	{
		vertexPos.push_back(glm::vec3(1, 1, 1));
		vertexPos.push_back(glm::vec3(1, 0, 1));
		vertexPos.push_back(glm::vec3(1, 1, 0));
		vertexPos.push_back(glm::vec3(1, 0, 0));
	}
	if (checkFace(Up, face))
	{
		vertexPos.push_back(glm::vec3(1, 1, 1));
		vertexPos.push_back(glm::vec3(1, 1, 0));
		vertexPos.push_back(glm::vec3(0, 1, 1));
		vertexPos.push_back(glm::vec3(0, 1, 0));
	}
	if (checkFace(Down, face))
	{
		vertexPos.push_back(glm::vec3(1, 0, 1));
		vertexPos.push_back(glm::vec3(1, 0, 0));
		vertexPos.push_back(glm::vec3(0, 0, 1));
		vertexPos.push_back(glm::vec3(0, 0, 0));
	}
	return vertexPos;
}

std::vector<unsigned char> Cube::getBrightness() const
{
	std::vector<unsigned char> brightness;
	if (face <= 0)
		return brightness;
	if (checkFace(Front, face))
	{
		for(int i=0;i<4;i++)
			brightness.push_back(0);
	}
	if (checkFace(Back, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(1);
	}
	if (checkFace(Left, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(3);
	}
	if (checkFace(Right, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(2);
	}
	if (checkFace(Up, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(4);
	}
	if (checkFace(Down, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(5);
	}
	return brightness;
}

std::vector<glm::vec3> Cube::getVertexTexture(int textureSides) const
{
#define addTexture1 texPos.push_back(glm::vec3(0, 0, 0));\
					texPos.push_back(glm::vec3(0, 1, 0));\
					texPos.push_back(glm::vec3(1, 0, 0));\
					texPos.push_back(glm::vec3(1, 1, 0));
#define addTexture2 texPos.push_back(glm::vec3(0, 0, 1));\
					texPos.push_back(glm::vec3(0, 1, 1));\
					texPos.push_back(glm::vec3(1, 0, 1));\
					texPos.push_back(glm::vec3(1, 1, 1));
#define addTexture3 texPos.push_back(glm::vec3(0, 0, 2));\
					texPos.push_back(glm::vec3(0, 1, 2));\
					texPos.push_back(glm::vec3(1, 0, 2));\
					texPos.push_back(glm::vec3(1, 1, 2));
	std::vector<glm::vec3> texPos;
	if (face <= 0)
		return texPos;
	if (checkFace(Front, face))
	{
		if (textureSides < 2)
		{
			addTexture1
		}
		else
		{
			addTexture2
		}
	}
	if (checkFace(Back, face))
	{
		if (textureSides < 2)
		{
			addTexture1
		}
		else
		{
			addTexture2
		}
	}
	if (checkFace(Left, face))
	{
		if (textureSides < 2)
		{
			addTexture1
		}
		else
		{
			addTexture2
		}
	}
	if (checkFace(Right, face))
	{
		if (textureSides < 2)
		{
			addTexture1
		}
		else
		{
			addTexture2
		}
	}
	if (checkFace(Up, face))
	{
		addTexture1
	}
	if (checkFace(Down, face))
	{
		if (textureSides < 3)
		{
			addTexture1
		}
		else
		{
			addTexture3
		}
	}

	return texPos;
}
int Cube::getVertexTexture(int texID,int textureSides,char dir)
{
	if (textureSides <= 1)
		return texID;
	if (checkFace(Front, dir))
	{
		if (textureSides < 2)
		{
			return texID;
		}
		else
		{
			return texID + 1;
		}
	}
	if (checkFace(Back, dir))
	{
		if (textureSides < 2)
		{
			return texID;
		}
		else
		{
			return texID + 1;
		}
	}
	if (checkFace(Left, dir))
	{
		if (textureSides < 2)
		{
			return texID;
		}
		else
		{
			return texID + 1;
		}
	}
	if (checkFace(Right, dir))
	{
		if (textureSides < 2)
		{
			return texID;
		}
		else
		{
			return texID + 1;
		}
	}
	if (checkFace(Up, dir))
	{
		return texID;
	}
	if (checkFace(Down, dir))
	{
		if (textureSides < 3)
		{
			return texID;
		}
		else
		{
			return texID + 2;
		}
	}

	return texID;
}

std::vector<GLuint> Cube::getIndex(bool doubleSides) const
{
#define addIndicesOrder1 index.push_back(startIndex + 0);index.push_back(startIndex + 2);\
						 index.push_back(startIndex + 1);index.push_back(startIndex + 1);\
						 index.push_back(startIndex + 2);index.push_back(startIndex + 3);
#define addIndicesOrder2 index.push_back(startIndex + 0);index.push_back(startIndex + 1);\
						 index.push_back(startIndex + 2);index.push_back(startIndex + 1);\
						 index.push_back(startIndex + 3);index.push_back(startIndex + 2);

	std::vector<GLuint> index;
	if (face <= 0)
		return index;
	int startIndex = 0;
	
	if (checkFace(Front, face))
	{
		if (doubleSides)
		{
			addIndicesOrder2
		}
		addIndicesOrder1
		startIndex += 4;
	}
	if (checkFace(Back, face))
	{
		if (doubleSides)
		{
			addIndicesOrder1
		}
		addIndicesOrder2
		startIndex += 4;
	}
	if (checkFace(Left, face))
	{
		if (doubleSides)
		{
			addIndicesOrder2
		}
		addIndicesOrder1

		startIndex += 4;
	}
	if (checkFace(Right, face))
	{
		if (doubleSides)
		{
			addIndicesOrder1
		}
		addIndicesOrder2
		startIndex += 4;
	}
	if (checkFace(Up, face))
	{
		if (doubleSides)
		{
			addIndicesOrder1
		}
		addIndicesOrder2
		startIndex += 4;
	}
	if (checkFace(Down, face))
	{
		if (doubleSides)
		{
			addIndicesOrder2
		}
		addIndicesOrder1
		startIndex += 4;
	}
	return index;
}

std::vector<GLuint> Cube::getVertex(int x, int y, int z, int textureSides, int texID, bool doubleSides)
{
	std::vector<glm::vec3> text = getVertexTexture(textureSides);
	std::vector<unsigned char> brightness = getBrightness();
	std::vector<glm::vec3> pos = getVertexPos();
	std::vector<GLuint> vertex;
	
	int n = std::min(text.size(), std::min(brightness.size(), pos.size()));

	for (int i = 0; i < n; i++)
	{
		vertex.push_back(
			(0b1111		 & ((int)pos[i].x + x))				+
			((0b11111111 & ((int)pos[i].y + y))		 << 4)	+
			((0b1111	 & ((int)pos[i].z + z))		 << 12) +
			((0b1		 & ((int)text[i].x))         << 16) +
			((0b1		 & ((int)text[i].y))         << 17) +
			((0b11111	 & (texID + (int)text[i].z))<< 18) +
			((0b111		 & ((int)brightness[i]))	 << 23));
	}
	return vertex;
}

GLuint Cube::getVertex(int x, int y, int z, int textureSides, int texID, int dir)
{
	return (x & 0b1111) +
		((y & 0b11111111) << 4) +
		((z & 0b1111) << 12) +
		((getVertexTexture(texID, textureSides, dir) & 0b111111) << 16);
}

GLuint Cube::indexSize() const
{
	GLuint indexS = 0;
	for (int i = 0; i < 6; i++)
	{
		if ((0b1 << i) & face)
			indexS++;
	}
	
	indexS *= 4;
	return indexS;
}