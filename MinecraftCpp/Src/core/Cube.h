#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;
class Engine;

enum class Faces :int
{
	Up    = 0b000001,
	Down  = 0b000010,
	Right = 0b000100,
	Left  = 0b001000,
	Front = 0b010000,
	Back  = 0b100000,
};
#define checkFace(face,myFace) ((int)Faces::face & (int)myFace) == (int)Faces::face

class Cube
{
	char face;
public:

	Cube();

	void draw(float x, float y, float z);

	void setFaceing(char faces);

	void setOneFace(char face, bool state = true);

	std::vector<glm::vec3> getVertexPos();

	std::vector<glm::vec2> getVertexTexture(int textureSides);

	std::vector<GLuint> getIndex();

	GLuint indexSize();

	friend class Engine;

};

