#include "VBO.h"


VBO::VBO(GLfloat* vertices, GLsizeiptr size, GLenum drawType)
{
	glGenBuffers(1, &ID);
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

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}