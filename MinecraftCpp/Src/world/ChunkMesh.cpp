#include "ChunkMesh.h"

ChunkMesh::ChunkMesh()
{
	vbo = new VBO();
	vao = new VAO();
	ebo = new EBO();
}

ChunkMesh::~ChunkMesh()
{
	delete vbo;
	delete vao;
	delete ebo;
}

void ChunkMesh::newMesh(std::vector<int> data, std::vector<GLuint> indices)
{
	vao->bind();
	vbo->setNewVertices(data);
	vao->linkData(*vbo, 0, 1, GL_FLOAT, sizeof(int), NULL);
	ebo->setNewIndices(indices);
	elements = indices.size();
	vao->unbind();
}

void ChunkMesh::draw()
{
	vao->bind();
	ebo->bind();
	glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
}
