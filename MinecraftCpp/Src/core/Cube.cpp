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
	1.0f / 3.0f, 1,
	1.0f / 3.0f, 0,
	2.0f / 3.0f, 1,
	2.0f / 3.0f, 0,
	///Lewo
	1.0f / 3.0f, 1,
	1.0f / 3.0f, 0,
	2.0f / 3.0f, 1,
	2.0f / 3.0f, 0,
	///Prawo
	1.0f / 3.0f, 1,
	1.0f / 3.0f, 0,
	2.0f / 3.0f, 1,
	2.0f / 3.0f, 0,
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
	///Front
	0,1,2,
	1,3,2,
};
static GLuint indicesBack[] = {
	///Ty³
	4,6,5,
	5,6,7,
};
static GLuint indicesLeft[] = {
	///Lewo
	8,9,10,
	9,11,10,
};
static GLuint indicesRight[] = {
	///Prawo
	12,14,13,
	13,14,15,
};
static GLuint indicesUp[] = {
	///Gora
	16,18,17,
	17,18,19,
};
static GLuint indicesDown[] = {
	///Dol
	20,21,22,
	22,21,23,
};


Shader* Cube::shader = NULL;
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
	shader = new Shader("Shader/Cube.vert", "Shader/Cube.geom", "Shader/Cube.frag");

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
	delete shader;
}

Cube::Cube(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Cube::draw()
{
	const int n = (up + down + left + right + front + back) * 6;
	if (n <= 0)
		return;
	shader->setUniformVec4(glm::vec4(1, 1, 1, 1), "modelColor");
	shader->setUniformVec3(glm::vec3(x, y, z), "pos");
	vao->bind();
	drawFaces();


}

void Cube::drawFaces()
{

	if (left)
	{
		eboLeft->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	if (right)
	{
		eboRight->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	if (front)
	{
		eboFront->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	if (back)
	{
		eboBack->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	if (up)
	{
		eboUp->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	if (down)
	{
		eboDown->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}