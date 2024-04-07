#include "Plant.h"
#include "Chunk.h"
#include "../scene/Game.h"

Plant::Plant(int ID, int x, int y, int z, int textureX, int textureY) :Block(ID, x, y, z)
{
	this->textureX = textureX;
	this->textureY = textureY;
}

void Plant::update(float deltaTime)
{

}



std::vector<GLuint> Plant::getVertex()
{
	int x = this->x;
	int z = this->z;
	if (x >= 0)
		x %= chunkW;
	else
		x = (chunkW - (abs(x) % chunkW)) % chunkW;
	if (z >= 0)
		z %= chunkT;
	else
		z = (chunkT - (abs(z) % chunkT)) % chunkT;
	std::vector<glm::vec3> pos = getPos(x,y,z);

	std::vector<glm::vec2> text{
	glm::vec2(textureX,textureY + 1),
	glm::vec2(textureX,textureY),
	glm::vec2(textureX + 1,textureY + 1),
	glm::vec2(textureX + 1,textureY),
	glm::vec2(textureX,textureY + 1),
	glm::vec2(textureX,textureY),
	glm::vec2(textureX + 1,textureY + 1),
	glm::vec2(textureX + 1,textureY),
	};

	std::vector<GLuint> vertices;
	for (int i = 0; i < pos.size(); i++)
	{

		vertices.push_back(
			(0b1111		 & ((int)pos[i].x))			+
			((0b11111111 & ((int)pos[i].y)) << 4)	+
			((0b1111     & ((int)pos[i].z)) << 12)	+
			((0b1111     & ((int)text[i].x)) << 16) +
			((0b1111     & ((int)text[i].y)) << 20) +
			  (0b11111111 << 24));
	}


	return vertices;
}

std::vector<GLuint> Plant::getIndex()
{
	std::vector<GLuint> index{
				0,1,2,
				0,2,1,
				1,2,3,
				1,3,2,

				4,5,6,
				4,6,5,
				5,6,7,
				5,7,6

	};


	return index;
}

GLuint Plant::indexSize()
{
	return 8;
}

std::vector<glm::vec3> Plant::getPos(int x, int y, int z)
{
	return {
		glm::vec3(x    ,y    ,z),
		glm::vec3(x    ,y + 1,z),
		glm::vec3(x + 1,y    ,z + 1),
		glm::vec3(x + 1,y + 1,z + 1),
		glm::vec3(x + 1,y    ,z),
		glm::vec3(x + 1,y + 1,z),
		glm::vec3(x    ,y    ,z + 1),
		glm::vec3(x    ,y + 1,z + 1),
	};
}