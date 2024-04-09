#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VBO.h"

class VAO
{
	GLuint ID;
public:
	VAO();

	~VAO();

	void linkData(VBO &vbo, GLuint index, GLuint size, GLenum type, GLsizeiptr stride, void* offset);

	void bind();

	void unbind();
};

