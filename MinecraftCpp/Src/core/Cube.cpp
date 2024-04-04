#include "Cube.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include "Engine.h"

Cube::Cube()
{
	face = 0b111111;
}

void Cube::setFaceing(char faces)
{
	this->face = faces;
}

void Cube::setOneFace(char face, bool state)
{
	char f = (char)this->face & ~((char)face);
	this->face = (f | ((char)face * state));
}

std::vector<glm::vec3> Cube::getVertexPos()
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

std::vector<unsigned char> Cube::getBrightness()
{
	std::vector<unsigned char> brightness;
	if (face <= 0)
		return brightness;
	if (checkFace(Front, face))
	{
		for(int i=0;i<4;i++)
			brightness.push_back(2);
	}
	if (checkFace(Back, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(2);
	}
	if (checkFace(Left, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(1);
	}
	if (checkFace(Right, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(1);
	}
	if (checkFace(Up, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(3);
	}
	if (checkFace(Down, face))
	{
		for (int i = 0; i < 4; i++)
			brightness.push_back(0);
	}
	return brightness;
}

std::vector<glm::vec2> Cube::getVertexTexture(int textureSides)
{
#define addTexture1 textPos.push_back(glm::vec2(0, 0));\
					textPos.push_back(glm::vec2(0, 1));\
					textPos.push_back(glm::vec2(1, 0));\
					textPos.push_back(glm::vec2(1, 1));
#define addTexture2 textPos.push_back(glm::vec2(1, 0));\
					textPos.push_back(glm::vec2(1, 1));\
					textPos.push_back(glm::vec2(2, 0));\
					textPos.push_back(glm::vec2(2, 1));
#define addTexture3 textPos.push_back(glm::vec2(2, 0));\
					textPos.push_back(glm::vec2(2, 1));\
					textPos.push_back(glm::vec2(3, 0));\
					textPos.push_back(glm::vec2(3, 1));
	std::vector<glm::vec2> textPos;
	//if (face <= 0)
	//	return textPos;
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

	return textPos;
}

std::vector<GLuint> Cube::getIndex(bool doubleSides)
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

std::vector<GLuint> Cube::getVertex(int x, int y, int z, int textureSides, int textX, int textY, bool doubleSides)
{
	std::vector<glm::vec2> text = getVertexTexture(textureSides);
	std::vector<unsigned char> brightness = getBrightness();
	std::vector<glm::vec3> pos = getVertexPos();
	std::vector<GLuint> vertex;
	int n = text.size();
	for (int i = 0; i < n; i++)
	{
		vertex.push_back(
			(0b1111		 & ((int)pos[i].x + x))				+
			((0b11111111 & ((int)pos[i].y + y))		 << 4)	+
			((0b1111	 & ((int)pos[i].z + z))		 << 12) +
			((0b1111	 & ((int)text[i].x + textX)) << 16) +
			((0b1111	 & ((int)text[i].y + textY)) << 20) +
			((0b11		 & ((int)brightness[i]))	 << 24));
	}

	return vertex;
}

GLuint Cube::indexSize()
{
	GLuint indexS = 0;
	if (((int)Faces::Front & (int)face) == (int)Faces::Front)
	{
		indexS++;
	}
	if (((int)Faces::Back & (int)face) == (int)Faces::Back)
	{
		indexS++;
	}
	if (((int)Faces::Left & (int)face) == (int)Faces::Left)
	{
		indexS++;
	}
	if (((int)Faces::Right & (int)face) == (int)Faces::Right)
	{
		indexS++;
	}
	if (((int)Faces::Up & (int)face) == (int)Faces::Up)
	{
		indexS++;
	}
	if (((int)Faces::Down & (int)face) == (int)Faces::Down)
	{
		indexS++;
	}
	indexS *= 4;
	return indexS;
}