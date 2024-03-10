#pragma once
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

class Rectangle
{
	VBO *colors;
	VBO* vertices;
	VAO vao;
	EBO *index;
public:
	Rectangle(float x, float y, float w, float h);

	~Rectangle();

	void draw();
};

