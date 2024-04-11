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

	void setFaceing(char faces);

	void setOneFace(char face, bool state = true);

	std::vector<glm::vec3> getVertexPos();

	std::vector<unsigned char> getBrightness();

	std::vector<glm::vec3> getVertexTexture(int textureSides);

	int getVertexTexture(int textureSides, char dir);

	std::vector<GLuint> getVertex(int x, int y, int z, int textureSides, int textID,bool doubleSides=false);

	GLuint getVertex(int x, int y, int z, int textureSides, int textID, int dir);

	std::vector<GLuint> getIndex(bool doubleSides = false);

	char getFaces()const { return face; }

	bool hasSide(int dir) { return ((int)(0b1 << dir) & (int)face) != (int)(0b1 << dir); }

	GLuint indexSize();

	friend class Engine;

};

