#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;
class Engine;

enum class Faces :char
{
	Right = 0b000001,
	Left  = 0b000010,
	Back  = 0b000100,
	Front = 0b001000,
	Up	  = 0b010000,
	Down  = 0b100000,
};
#define checkFace(face,myFace) ((int)Faces::face & (int)myFace) == (int)Faces::face

class Cube
{
	char face;
public:

	Cube();

	void setFacing(char faces);

	void setOneFace(char face, bool state = true);

	int getVertexTexture(int texID, int textureSides, char dir);

	GLuint getVertex(int x, int y, int z, int textureSides, int texID, int dir);

	char getFaces()const { return face; }

	bool hasSide(int dir) { return ((int)(0b1 << dir) & (int)face) != (int)(0b1 << dir); }

	friend class Engine;

};
