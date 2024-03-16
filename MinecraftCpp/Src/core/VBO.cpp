#include "VBO.h"
#include "../Properties.h"

VBO::VBO()
{
	glGenBuffers(1, &ID);
#ifdef DebugInfoMode
#define info
	printf("[INFO]: Succesful Create VBO %d\n", (int)ID);
#endif
}

VBO::VBO(std::vector<glm::vec3>& vertices):VBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
}
VBO::VBO(std::vector<glm::vec2>& vertices) :VBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);
}
VBO::VBO(GLfloat* vertices, GLsizeiptr size, GLenum drawType) :VBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, drawType);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &ID);
}

void VBO::setNewVertices(GLfloat* vertices, GLsizeiptr size)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

void VBO::setNewVertices(std::vector<glm::vec3>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
}
void VBO::setNewVertices(std::vector<glm::vec2>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);
}

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}