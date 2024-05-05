#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
class Camera;
class Shader;
class VAO;
class VBO;
class EBO;

class SkyBox
{
	VAO* vao = NULL;
	VBO* vbo = NULL;
	EBO* ebo = NULL;
	Shader* shader = NULL;
	GLuint cubeTextur;
public:
	SkyBox(const char* path);

	~SkyBox();

	void draw(Camera* camera);

};

