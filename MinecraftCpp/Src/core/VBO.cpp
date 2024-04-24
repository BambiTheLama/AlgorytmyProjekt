#include "VBO.h"
#include "../Properties.h"

VBO::VBO()
{
	glGenBuffers(1, &ID);
#ifdef DebugInfoMode
	printf("[INFO]: Succesful Create VBO %d\n", (int)ID);
#endif
}

VBO::VBO(GLfloat* vertices, GLsizeiptr size, GLenum drawType) :VBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, drawType);
}

VBO::~VBO()
{
#ifdef DebugInfoMode
	printf("[INFO]: Succesful DELETE VBO %d\n", (int)ID);
#endif
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
void VBO::setNewVertices(std::vector<glm::vec4>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), vertices.data(), GL_DYNAMIC_DRAW);
}

void VBO::setNewVertices(std::vector<int>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(int), vertices.data(), GL_DYNAMIC_DRAW);
}
void VBO::setNewVertices(std::vector<float>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
}

void VBO::setNewVertices(std::vector<GLuint>& vertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLuint), vertices.data(), GL_DYNAMIC_DRAW);
}

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}