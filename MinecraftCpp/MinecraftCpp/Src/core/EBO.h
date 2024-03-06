#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class EBO
{
	GLuint ID;
public:
	EBO(GLuint* indices, GLsizeiptr size);

	~EBO();

	void bind();

	void unbind();
};

