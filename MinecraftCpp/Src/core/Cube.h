#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
class Shader;
class Engine;

class Cube
{
	static Shader* shader;
	float x, y, z;

public:
	bool up = true, down = true, left = true, right = true, front = true, back = true;
	Cube(float x, float y, float z);

	void draw();

	friend class Engine;
private:
	void drawFaces();
	void setUpIndices(const int n, GLuint* indices);
	void addIndices(bool leftSide,int index,int& i, GLuint* indices);
	static void CubeSetUp();
	static void CubeDelete();
	glm::vec3 getPos() { return glm::vec3(x, y, z); }

};

