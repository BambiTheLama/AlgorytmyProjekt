#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Engine
{
	float height, width;
	float delataTime = 0.0f;
	std::string title;
	GLFWwindow* window;
	
public:
	Engine();

	~Engine();

	void start();

	float getDeltaTime()const { return delataTime; }
};

