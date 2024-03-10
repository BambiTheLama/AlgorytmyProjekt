#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VBO
{
	GLuint ID;
public:
	VBO(GLfloat* vertices, GLsizeiptr size, GLenum drawType = GL_STATIC_DRAW);

	~VBO();

	void setNewVertices(GLfloat* vertices, GLsizeiptr size);

	void bind();

	void unbind();



};

