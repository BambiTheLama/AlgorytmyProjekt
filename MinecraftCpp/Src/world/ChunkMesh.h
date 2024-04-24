#pragma once
#include "../core/EBO.h"
#include "../core/VAO.h"

class Shader;
class ChunkMesh
{
	VAO* vao = NULL;
	VBO* vbo = NULL;
	std::vector<int> data;
	int elements = 0;
	int dir = 0;

public:
	ChunkMesh(int dir);

	~ChunkMesh();

	void start();

	void newMesh(std::vector<int> data);
	
	void clearMesh();

	void addData(std::vector<int> data);

	void genMesh();

	std::vector<int> getData() { return data; }

	void draw(Shader* s);
};