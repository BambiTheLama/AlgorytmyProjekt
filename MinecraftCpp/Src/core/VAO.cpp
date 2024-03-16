#include "VAO.h"
#include "VBO.h"
#include "../Properties.h"
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
#ifdef DebugInfoMode
#define info
	printf("[INFO]: Succesful Create VAO %d\n", (int)ID);
#endif

}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &ID);
}

void VAO::linkData(VBO &vbo, GLuint index, GLuint size, GLenum type, GLsizeiptr stride, void* offset)
{
	vbo.bind();
	glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(index);
	vbo.unbind();
}

void VAO::bind()
{
	glBindVertexArray(ID);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}