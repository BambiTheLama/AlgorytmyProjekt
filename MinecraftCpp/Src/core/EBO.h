#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
class EBO
{
	GLuint ID;
public:
	EBO(std::vector<GLuint>& indices);

	EBO(GLuint* indices, GLsizeiptr size, GLenum drawType = GL_STATIC_DRAW);

	~EBO();

	void setNewIndices(GLuint* indices, GLsizeiptr size);

	void setNewVertices(std::vector<GLuint>& indices);

	void bind();

	void unbind();
};

