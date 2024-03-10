#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class EBO
{
	GLuint ID;
public:
	EBO(GLuint* indices, GLsizeiptr size, GLenum drawType = GL_STATIC_DRAW);

	~EBO();

	void setNewIndices(GLuint* indices, GLsizeiptr size);

	void bind();

	void unbind();
};

