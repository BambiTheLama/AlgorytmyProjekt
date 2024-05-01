#pragma once
#include "../core/EBO.h"
#include "../core/VAO.h"

class Shader;
class ChunkMesh
{
	VAO* vao = NULL;
	VBO* vbo = NULL;
	std::vector<glm::uvec2> data;

	int elements = 0;
	int dir = 0;

public:
	ChunkMesh(int dir);

	~ChunkMesh();

	void start();

	void newMesh(std::vector<glm::uvec2> data);
	
	void clearMesh();

	void addData(std::vector<glm::uvec2> data);

	void genMesh();

	std::vector<glm::uvec2> getData() { return data; }

	void draw(Shader* s);
};
