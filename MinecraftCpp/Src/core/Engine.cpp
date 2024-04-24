#include "Engine.h"
#include <iostream>
#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Texture.h"
#include <glm/gtx/transform.hpp>
#include <stb/stb_image.h>
#include <vector>
#include "../World/Chunk.h"
#include "../scene/Game.h"
#include "Font.h"
#include "RenderTexture.h"

static Shader* textShader = NULL;

static Engine* e;

void reside(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	e->height = height;
	e->width = width;
	
}

Engine::Engine()
{
	height = 900;
	width = 1600;
	title = "MineGL";

	if (!glfwInit())
		return;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	stbi_set_flip_vertically_on_load(false);

	window = glfwCreateWindow(width, height, title.data(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, reside);
	glfwSwapInterval(0);
	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	Font::setUpFonts();
	Font::setScreenSize(width, height);

	RenderTexture::setUpRenderTextures();

	e = this;
	start();
}

Engine::~Engine()
{
	RenderTexture::endRenderTexture();
	Font::freeFonts();
	Texture::clearAllTextures();

	glfwDestroyWindow(window);
	glfwTerminate();
	e = NULL;
}

void diffViewport()
{
	glViewport(0, 0, e->width, e->height);
}

void Engine::start()
{

	float lastTime = glfwGetTime();	
	Font f("Res/ComicStans.ttf");
	Game* game = new Game(width, height, window);
	std::string fps;
	std::vector<float> times;
	float changeText = 0.0;
	game->start();

	float time=0;
	while (!glfwWindowShouldClose(window))
	{

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		time += deltaTime;
		lastTime = currentTime;
		changeText -= deltaTime;
		times.push_back(deltaTime);
		if (changeText <= 0)
		{
			float dt = 0;
			for (auto t : times)
				dt += t;
			dt /= times.size();
			fps = std::string(std::to_string((int)(1.0f / dt)) + " FPS\n{c:255,0,0}F{c:0,255,0}P{c:0,0,255}S");
			changeText = 0.2137f;
			times.clear();
		}

		game->update(deltaTime);

		glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game->draw();
		Font::setScreenSize(width, height);
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);
		f.drawText(fps, 0, 0, 1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		glEnable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	delete game;
}

