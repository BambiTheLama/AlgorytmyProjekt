#include "Plant.h"
#include "../Chunk.h"
#include "../../scene/Game.h"

Plant::Plant(int ID, int x, int y, int z, int textureID) :Block(ID, x, y, z)
{
	this->textureID = textureID;
}

void Plant::update(float deltaTime)
{

}

GLuint Plant::getVertex(int dir)
{
	return (x & 0b1111) +
		((y & 0b11111111) << 4) +
		((z & 0b1111) << 12) +
		((textureID & 0b111111) << 16)+ 
		(0b1 << 24);
}

std::vector<glm::vec3> Plant::getPos(int x, int y, int z)
{
	return {
		glm::vec3(x, y, z),
		glm::vec3(x, y + 1, z),
		glm::vec3(x + 1, y, z + 1),
		glm::vec3(x + 1, y + 1, z + 1),
		glm::vec3(x + 1, y, z),
		glm::vec3(x + 1, y + 1, z),
		glm::vec3(x, y, z + 1),
		glm::vec3(x, y + 1, z + 1),
	};
}