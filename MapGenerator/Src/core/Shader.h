#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Shader
{
	GLuint ID;
public:
	Shader(const char* vertex, const char* frag);

	~Shader();

	void active();

	GLint getUniformLocation(const char* uniform);
};

