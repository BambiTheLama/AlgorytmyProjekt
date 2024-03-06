#include "Engine.h"
#include <iostream>
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Shader.h"
#include "Rectangle.h"
#include "Camera.h"

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

	Shader Shader("Src/Shader/Dif.vert", "Src/Shader/Dif.frag");
	Rectangle rec(0, 0, 1, 0.7f);
	Camera camera(width, height, 0.1f, 100, 45, glm::vec3(0.0f, 0.0f, 2.0f));

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.updatePos(glm::vec3(0.0f,0.0f,1.0f*deltaTime));
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.updatePos(glm::vec3(0.0f, 0.0f, -1.0f * deltaTime));

		glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Shader.active();
		glUniformMatrix4fv(Shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		camera.useCamera(Shader, "camera");

		rec.draw();
		glfwSwapBuffers(window);

		glfwPollEvents();
	}
}