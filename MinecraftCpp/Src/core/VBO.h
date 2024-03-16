#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
class VBO
{
	GLuint ID;
public:
	VBO();

	VBO(std::vector<glm::vec3>& vertices);

	VBO(std::vector<glm::vec2>& vertices);

	VBO(GLfloat* vertices, GLsizeiptr size, GLenum drawType = GL_STATIC_DRAW);

	~VBO();

	void setNewVertices(GLfloat* vertices, GLsizeiptr size);

	void setNewVertices(std::vector<glm::vec3>& vertices);

	void setNewVertices(std::vector<glm::vec2>& vertices);

	void bind();

	void unbind();



};

