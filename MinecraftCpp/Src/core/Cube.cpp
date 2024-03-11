#include "Cube.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include "Engine.h"

static GLfloat verticies[] = {
	///Front
	0.5f,0.5f,0.5f,
	0.5f,-0.5f,0.5f,
	-0.5f,0.5f,0.5f,
	-0.5f,-0.5f,0.5f,
	///Ty³
	0.5f,0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	-0.5f,0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	///Lewo
	-0.5f,0.5f,0.5f,
	-0.5f,-0.5f,0.5f,
	-0.5f,0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	///Prawo
	0.5f,0.5f,0.5f,
	0.5f,-0.5f,0.5f,
	0.5f,0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	///Gora
	0.5f,0.5f,0.5f,
	0.5f,0.5f,-0.5f,
	-0.5f,0.5f,0.5f,
	-0.5f,0.5f,-0.5f,
	///Dol
	0.5f,-0.5f,0.5f,
	0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,0.5f,
	-0.5f,-0.5f,-0.5f,
};
static GLfloat texture[] = {
	///Front
	1.0f / 3.0f, 1,
	1.0f / 3.0f, 0,
	2.0f / 3.0f, 1,
	2.0f / 3.0f, 0,
	///Ty³
	2.0f / 3.0f, 1,
	2.0f / 3.0f, 0,
	1.0f / 3.0f, 1,
	1.0f / 3.0f, 0,
	///Lewo
	1.0f / 3.0f, 1,
	1.0f / 3.0f, 0,
	2.0f / 3.0f, 1,
	2.0f / 3.0f, 0,
	///Prawo
	2.0f / 3.0f, 1,
	2.0f / 3.0f, 0,
	1.0f / 3.0f, 1,
	1.0f / 3.0f, 0,
	///Gora
	0.0f / 3.0f, 1,
	0.0f / 3.0f, 0,
	1.0f / 3.0f, 1,
	1.0f / 3.0f, 0,
	///Dol
	2.0f / 3.0f, 1,
	2.0f / 3.0f, 0,
	3.0f / 3.0f, 1,
	3.0f / 3.0f, 0,
};

static GLuint indicesFront[] = {
	0,2,1,
	1,2,3,
};
static GLuint indicesBack[] = {
	4,5,6,
	6,5,7,
};
static GLuint indicesLeft[] = {
	8,10,9,
	9,10,11,
};
static GLuint indicesRight[] = {

	12,13,14,
	13,15,14,
};
static GLuint indicesUp[] = {

	16,17,18,
	17,19,18,
};
static GLuint indicesDown[] = {
	20,22,21,
	21,22,23,
};

static VAO* vao = NULL;
static VBO* vboVertices = NULL;
static VBO* vboTexture = NULL;
static EBO* eboUp = NULL;
static EBO* eboDown = NULL;
static EBO* eboLeft = NULL;
static EBO* eboRight = NULL;
static EBO* eboFront = NULL;
static EBO* eboBack = NULL;

void Cube::CubeSetUp()
{
	vao = new VAO();
	vao->bind();
	vboVertices = new VBO(verticies, sizeof(verticies));
	vboTexture = new VBO(texture, sizeof(texture));
	eboUp = new EBO(indicesUp, sizeof(indicesUp));
	eboDown = new EBO(indicesDown, sizeof(indicesDown));
	eboLeft = new EBO(indicesLeft, sizeof(indicesLeft));
	eboRight = new EBO(indicesRight, sizeof(indicesRight));
	eboFront = new EBO(indicesFront, sizeof(indicesFront));
	eboBack = new EBO(indicesBack, sizeof(indicesBack));
	vao->linkData(*vboVertices, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	vao->linkData(*vboTexture, 1, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
	vboVertices->unbind();
	vao->unbind();
	

}

void Cube::CubeDelete()
{
	delete vao;
	delete vboVertices;
	delete eboUp;
	delete eboDown;
	delete eboLeft; 
	delete eboRight; 
	delete eboFront;
	delete eboBack; 
}

Cube::Cube()
{
	face = 0b111111;
}

void Cube::draw(float x, float y, float z)
{
	Shader& s = getDiffoltShader();
	vao->bind();
	s.setUniformVec4(glm::vec4(1, 1, 1, 1), "modelColor");
	s.setUniformVec3(glm::vec3(x, y, z), "pos");
	drawFaces();
}

void Cube::setFaceing(int faces)
{
	this->face = faces;
}

void Cube::setOneFace(int face, bool state)
{
	int f = (int)this->face & ~((int)face);
	this->face = (f | ((int)face * state));
}

void Cube::drawFaces()
{
	const int n = 6;
	if (((int)Faces::Left & (int)face) == (int)Faces::Left)
	{
		eboLeft->bind();
		glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
	}
	if (((int)Faces::Right & (int)face) == (int)Faces::Right)
	{
		eboRight->bind();
		glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
	}
	if (((int)Faces::Front & (int)face) == (int)Faces::Front)
	{
		eboFront->bind();
		glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
	}
	if (((int)Faces::Back & (int)face) == (int)Faces::Back)
	{
		eboBack->bind();
		glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
	}
	if (((int)Faces::Up & (int)face) == (int)Faces::Up)
	{
		eboUp->bind();
		glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
	}
	if (((int)Faces::Down & (int)face) == (int)Faces::Down)
	{
		eboDown->bind();
		glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
	}
}