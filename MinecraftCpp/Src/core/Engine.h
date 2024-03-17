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

	friend void drawTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color);
	
};

void startShaderMode(Shader& s);

void endShaderMode();

Shader& getDiffoltShader();

Texture* getBlocks();
