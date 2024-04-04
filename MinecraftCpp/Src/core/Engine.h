#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>
class Cube;
class Shader;
class Chunk;
class Texture;

class Engine
{
	float height, width;
	float deltaTime = 0.0f;
	std::string title;
	GLFWwindow* window;
	
public:
	Engine();

	~Engine();

	void start();

	float getDeltaTime()const { return deltaTime; }

	void diffViewport();

	friend void reside(GLFWwindow* window, int width, int height);

	friend void diffViewport();

};

void diffViewport();