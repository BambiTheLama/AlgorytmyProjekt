#include "Plant.h"
#include "Chunk.h"
#include "../scene/Game.h"

Plant::Plant(int ID, int x, int y, int z, int textureID) :Block(ID, x, y, z)
{
	this->textureID = textureID;
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
	glm::vec2(0,1),
	glm::vec2(0,0),
	glm::vec2(1,1),
	glm::vec2(1,0),
	glm::vec2(0,1),
	glm::vec2(0,0),
	glm::vec2(1,1),
	glm::vec2(1,0),
	};

	std::vector<GLuint> vertices;
	for (int i = 0; i < pos.size(); i++)
	{

		vertices.push_back(
			(0b1111		 & ((int)pos[i].x))			+
			((0b11111111 & ((int)pos[i].y))  << 4)	+
			((0b1111     & ((int)pos[i].z))  << 12)	+
			((0b1	     & ((int)text[i].x)) << 16) +
			((0b1	     & ((int)text[i].y)) << 17) +
			((0b11111	 & textureID)		 << 18));
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