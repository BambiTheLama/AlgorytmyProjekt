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

GLuint Cube::getVertex(int x, int y, int z, int textureSides, int texID, int dir)
{
	return (x & 0b11111) +
		((y & 0b11111111) << 5) +
		((z & 0b11111) << 13) +
		((getVertexTexture(texID, textureSides, dir) & 0b111111) << 18);
}

