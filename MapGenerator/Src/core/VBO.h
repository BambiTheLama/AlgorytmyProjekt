#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VBO
{
	GLuint ID;
public:
	VBO(GLfloat* vertices, GLsizeiptr size);

	~VBO();

	void bind();

	void unbind();



};

