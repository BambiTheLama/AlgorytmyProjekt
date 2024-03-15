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
	if (((int)Faces::Front & (int)face) == (int)Faces::Front)
	{
		vertexPos.push_back(glm::vec3(1, 1, 1));
		vertexPos.push_back(glm::vec3(1, 0, 1));
		vertexPos.push_back(glm::vec3(0, 1, 1));
		vertexPos.push_back(glm::vec3(0, 0, 1));
	}
	if (((int)Faces::Back & (int)face) == (int)Faces::Back)
	{
		vertexPos.push_back(glm::vec3(1, 1, 0));
		vertexPos.push_back(glm::vec3(1, 0, 0));
		vertexPos.push_back(glm::vec3(0, 1, 0));
		vertexPos.push_back(glm::vec3(0, 0, 0));
	}
	if (((int)Faces::Left & (int)face) == (int)Faces::Left)
	{
		vertexPos.push_back(glm::vec3(0, 1, 1));
		vertexPos.push_back(glm::vec3(0, 0, 1));
		vertexPos.push_back(glm::vec3(0, 1, 0));
		vertexPos.push_back(glm::vec3(0, 0, 0));
	}
	if (((int)Faces::Right & (int)face) == (int)Faces::Right)
	{
		vertexPos.push_back(glm::vec3(1, 1, 1));
		vertexPos.push_back(glm::vec3(1, 0, 1));
		vertexPos.push_back(glm::vec3(1, 1, 0));
		vertexPos.push_back(glm::vec3(1, 0, 0));
	}
	if (((int)Faces::Up & (int)face) == (int)Faces::Up)
	{
		vertexPos.push_back(glm::vec3(1, 1, 1));
		vertexPos.push_back(glm::vec3(1, 1, 0));
		vertexPos.push_back(glm::vec3(0, 1, 1));
		vertexPos.push_back(glm::vec3(0, 1, 0));
	}
	if (((int)Faces::Down & (int)face) == (int)Faces::Down)
	{
		vertexPos.push_back(glm::vec3(1, 0, 1));
		vertexPos.push_back(glm::vec3(1, 0, 0));
		vertexPos.push_back(glm::vec3(0, 0, 1));
		vertexPos.push_back(glm::vec3(0, 0, 0));
	}
	return vertexPos;
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
	if (face <= 0)
		return textPos;

	if (((int)Faces::Front & (int)face) == (int)Faces::Front)
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
	if (((int)Faces::Back & (int)face) == (int)Faces::Back)
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
	if (((int)Faces::Left & (int)face) == (int)Faces::Left)
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
	if (((int)Faces::Right & (int)face) == (int)Faces::Right)
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
	if (((int)Faces::Up & (int)face) == (int)Faces::Up)
	{
		addTexture1
	}
	if (((int)Faces::Down & (int)face) == (int)Faces::Down)
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

std::vector<GLuint> Cube::getIndex()
{
#define addIndicesOrder1 index.push_back(startIndex + 0);index.push_back(startIndex + 2);\
						 index.push_back(startIndex + 1);index.push_back(startIndex + 1);\
						 index.push_back(startIndex + 2);index.push_back(startIndex + 3);startIndex += 4;
#define addIndicesOrder2 index.push_back(startIndex + 0);index.push_back(startIndex + 1);\
						 index.push_back(startIndex + 2);index.push_back(startIndex + 1);\
						 index.push_back(startIndex + 3);index.push_back(startIndex + 2);startIndex += 4;

	std::vector<GLuint> index;
	if (face <= 0)
		return index;
	int startIndex = 0;

	if (((int)Faces::Front & (int)face) == (int)Faces::Front)
	{
		addIndicesOrder1
	}
	if (((int)Faces::Back & (int)face) == (int)Faces::Back)
	{
		addIndicesOrder2
	}
	if (((int)Faces::Left & (int)face) == (int)Faces::Left)
	{
		addIndicesOrder1
	}
	if (((int)Faces::Right & (int)face) == (int)Faces::Right)
	{
		addIndicesOrder2
	}
	if (((int)Faces::Up & (int)face) == (int)Faces::Up)
	{
		addIndicesOrder2
	}
	if (((int)Faces::Down & (int)face) == (int)Faces::Down)
	{
		addIndicesOrder1
	}
	return index;
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