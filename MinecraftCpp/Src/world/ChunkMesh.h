#pragma once
#include "../core/EBO.h"
#include "../core/VAO.h"

class Shader;
class ChunkMesh
{
	VAO* vao = NULL;
	VBO* vbo = NULL;
	int elements;
	int dir;
public:
	ChunkMesh(int dir);

	~ChunkMesh();

	void newMesh(std::vector<int> data);

	void draw(Shader* s);
};

