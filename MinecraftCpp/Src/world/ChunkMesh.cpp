#include "ChunkMesh.h"
#include "../core/Shader.h"

static VBO* vboQuad = NULL;
static std::vector<glm::vec3> vertQuad = {

	//Front	
	glm::vec3(1,0,0),
	glm::vec3(1,1,0),
	glm::vec3(1,0,1),
	glm::vec3(1,1,1),
	//Back
	glm::vec3(0,0,0),
	glm::vec3(0,0,1),
	glm::vec3(0,1,0),
	glm::vec3(0,1,1),
	//Left
	glm::vec3(0,0,0),
	glm::vec3(0,1,0),
	glm::vec3(1,0,0),
	glm::vec3(1,1,0),
	//Right
	glm::vec3(0,0,1),
	glm::vec3(1,0,1),
	glm::vec3(0,1,1),
	glm::vec3(1,1,1),
	//UP
	glm::vec3(0,1,0),
	glm::vec3(0,1,1),
	glm::vec3(1,1,0),
	glm::vec3(1,1,1),
	//Down
	glm::vec3(0,0,0),
	glm::vec3(1,0,0),
	glm::vec3(0,0,1),
	glm::vec3(1,0,1),
	//XFirst
	glm::vec3(0,0,0),
	glm::vec3(0,1,0),
	glm::vec3(1,0,1),
	glm::vec3(1,1,1),
	//XSec
	glm::vec3(0,0,0),
	glm::vec3(1,0,1),
	glm::vec3(0,1,0),
	glm::vec3(1,1,1),
	//X3
	glm::vec3(0,0,1),
	glm::vec3(0,1,1),
	glm::vec3(1,0,0),
	glm::vec3(1,1,0),
	//X4
	glm::vec3(0,0,1),
	glm::vec3(1,0,0),
	glm::vec3(0,1,1),
	glm::vec3(1,1,0),
};
static VBO* vboQuadText = NULL;
static std::vector<glm::vec2> vertQuadText = {
	//Front	
	glm::vec2(0,1),
	glm::vec2(0,0),
	glm::vec2(1,1),
	glm::vec2(1,0),
	//Back
	glm::vec2(0,1),
	glm::vec2(1,1),
	glm::vec2(0,0),
	glm::vec2(1,0),
	//Left
	glm::vec2(0,1),
	glm::vec2(0,0),
	glm::vec2(1,1),
	glm::vec2(1,0),
	//Right
	glm::vec2(0,1),
	glm::vec2(1,1),
	glm::vec2(0,0),
	glm::vec2(1,0),
	//UP
	glm::vec2(0,1),
	glm::vec2(1,1),
	glm::vec2(0,0),
	glm::vec2(1,0),
	//Down
	glm::vec2(0,0),
	glm::vec2(1,0),
	glm::vec2(0,1),
	glm::vec2(1,1),
	//X1
	glm::vec2(0,1),
	glm::vec2(0,0),
	glm::vec2(1,1),
	glm::vec2(1,0),
	//X2
	glm::vec2(0,1),
	glm::vec2(1,1),
	glm::vec2(0,0),
	glm::vec2(1,0),
	//X3
	glm::vec2(0,1),
	glm::vec2(0,0),
	glm::vec2(1,1),
	glm::vec2(1,0),
	//X4
	glm::vec2(0,1),
	glm::vec2(1,1),
	glm::vec2(0,0),
	glm::vec2(1,0),
};

ChunkMesh::ChunkMesh(int dir)
{
	this->dir = dir;
}

ChunkMesh::~ChunkMesh()
{
	if(vbo)	
		delete vbo;
	if(vao)
		delete vao;
}

void ChunkMesh::start()
{
	if (!vboQuad)
	{
		vboQuad = new VBO();
		vboQuad->setNewVertices(vertQuad);
	}
	if (!vboQuadText)
	{
		vboQuadText = new VBO();
		vboQuadText->setNewVertices(vertQuadText);
	}
	vbo = new VBO();
	vao = new VAO();
	vao->bind();
	vao->linkData(*vboQuad, 1, 3, GL_FLOAT, sizeof(glm::vec3), NULL);
	vao->linkData(*vboQuadText, 2, 2, GL_FLOAT, sizeof(glm::vec2), NULL);
}

void ChunkMesh::newMesh(std::vector<int> data)
{
	vao->bind();
	vbo->setNewVertices(data);
	vao->linkData(*vbo, 0, 1, GL_FLOAT, sizeof(int), NULL);
	glVertexAttribDivisor(0, 1);
	vao->unbind();

	this->elements = data.size();

}
void ChunkMesh::clearMesh()
{
	this->elements = 0;
	data.clear();
}

void ChunkMesh::addData(std::vector<int> data)
{
	this->data.insert(this->data.end(), data.begin(), data.end());
}

void ChunkMesh::genMesh()
{
	vao->bind();
	vbo->setNewVertices(data);
	vao->linkData(*vbo, 0, 1, GL_FLOAT, sizeof(int), NULL);
	glVertexAttribDivisor(0, 1);
	vao->unbind();
	this->elements = data.size();
}

void ChunkMesh::draw(Shader* s)
{
	if (elements <= 0)
		return;
	vao->bind();

	s->setUniformI1(dir, "dir");
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, dir * 4, 4, elements);
}
