#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
class Cube;
class Shader;
class Chunk;
class Texture;
class Camera;

class Engine
{
	float height, width;
	float deltaTime = 0.0f;
	std::string title;
	GLFWwindow* window;
	ImGuiIO* io;
	
public:
	Engine();

	~Engine();

	void start();

	float getDeltaTime()const { return deltaTime; }

	void diffViewport();

	friend void resize(GLFWwindow* window, int width, int height);

	friend void diffViewport();

};

void diffViewport();

void drawCubeAt(int x, int y, int z, Camera* camera, char faces=0b111111);