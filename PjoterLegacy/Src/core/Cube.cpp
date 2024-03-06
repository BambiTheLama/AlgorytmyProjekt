#include "Cube.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLfloat verticies[] = {
	1,1,1,
	1,-1,1,
	-1,1,1,
	-1,-1,1,
	1,1,-1,
	1,-1,-1,
	-1,1,-1,
	-1,-1,-1,
};
GLfloat colos[] = {
	1,1,1,
	1,1,1,
	1,1,1,
	1,1,1,
	1,1,1,
	1,1,1,
	1,1,1,
	1,1,1,
};

GLuint indices[] = {
	0,1,2,
	1,3,2,
	4,6,5,
	5,6,7,
	0,4,1,
	1,4,5,
	2,3,6,
	3,7,6,
	0,2,4,
	2,6,4,
	1,5,3,
	3,5,7,

};
VAO* vao;
VBO* vboVertices;
VBO* vboColors;
EBO* ebo;

void Cube::CubeSetUp()
{
	vao = new VAO();
	vao->bind();
	vboVertices = new VBO(verticies, sizeof(verticies));
	vboColors = new VBO(colos, sizeof(colos));
	ebo = new EBO(indices, sizeof(indices));
	vao->linkData(*vboVertices, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	vao->linkData(*vboColors, 1, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	vboVertices->unbind();
	vboColors->unbind();
	ebo->unbind();
	vao->unbind();

}

void Cube::CubeDelete()
{
	delete vao;
	delete vboVertices;
	delete vboColors;
	delete ebo;
}

Cube::Cube(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Cube::draw(Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, z));
	shader.setUniformMat4(model, "model");
	shader.setUniformVec4(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),"modelColor");
	vao->bind();
	ebo->bind();
	//printf("%d\n", sizeof(indices) / sizeof(GLuint));
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}