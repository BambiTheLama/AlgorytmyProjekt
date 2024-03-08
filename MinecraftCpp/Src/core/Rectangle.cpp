#include "Rectangle.h"


Rectangle::Rectangle(float x, float y, float w, float h)
{
	GLfloat ver[] = {
		x,y,0.0f,
		x,y + h,0.0f,
		x + w,y,0.0f,
		x + w,y + h,0.0f,
	};
	GLfloat tex[] = {
		0,0,
		0,1,
		1,0,
		1,1,
	};
	GLuint ind[] = {
		0,2,1,
		3,1,2
	};

	vao.bind();
	colors = new VBO(tex, sizeof(tex));
	vertices = new VBO(ver, sizeof(ver));
	vao.linkData(*vertices, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	vao.linkData(*colors, 1, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
	index = new EBO(ind, sizeof(ind));

	vao.unbind();
	colors->unbind();
	vertices->unbind();
	index->unbind();
}

Rectangle::~Rectangle()
{
	delete colors;
	delete vertices;
	delete index;
}

void Rectangle::draw()
{
	vao.bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}