#include "Engine.h"
#include <iostream>

Engine::Engine()
{
	height = 600;
	width = 900;
	title = "minecraft";

	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, title.data(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);
	//glfwSetCursorPosCallback(window, OnMouse);

	//GlCall(glEnable(GL_DEPTH_TEST));
	//GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//GlCall(glEnable(GL_BLEND));

	glFrontFace(GL_CW);
	// Cull back faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

Engine::~Engine()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::start()
{
	float lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		delataTime = currentTime - lastTime;
		lastTime = currentTime;


		glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}