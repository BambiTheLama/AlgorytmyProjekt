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

glm::uvec2 Plant::getVertex(int dir)
{
	return glm::uvec2((x & 0b11111) +
		((y & 0b11111111) << 5) +
		((z & 0b11111) << 13) +
		((textureID & 0b111111) << 18), 4);
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