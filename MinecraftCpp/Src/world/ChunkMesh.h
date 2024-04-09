#pragma once
#include "../core/EBO.h"
#include "../core/VAO.h"

class ChunkMesh
{
	VAO* vao = NULL;
	VBO* vbo = NULL;
	EBO* ebo = NULL;
	int elements;
public:
	ChunkMesh();

	~ChunkMesh();

	void newMesh(std::vector<int> data, std::vector<GLuint> indices);

	void draw();
};

