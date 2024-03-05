#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Shaider
{
	GLuint ID;
public:
	Shaider(const char* vertex, const char* frag);

	~Shaider();

	void active();

};

