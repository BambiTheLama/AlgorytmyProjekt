#include "EBO.h"


EBO::EBO(GLuint* indices, GLsizeiptr size, GLenum drawType)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, drawType);
}

EBO::~EBO()
{
	glDeleteBuffers(1, &ID);
}
void EBO::setNewIndices(GLuint* indices, GLsizeiptr size)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_DYNAMIC_DRAW);
}
void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}