#include "Engine.h"
#include <iostream>
#include "Rectangle.h"
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

static Shader* shader = NULL;
static Shader* textShader = NULL;
static Camera* camera = NULL;
static Texture* blocks = NULL;
static Texture* blocksH = NULL;
static Texture* blocksN = NULL;

void reside(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

}

Engine::Engine()
{
	height = 900;
	width = 1600;
	title = "minecraft";

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
	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	shader = new Shader("Shader/Diff.vert", "Shader/Diff.geom", "Shader/Diff.frag");
	camera = new Camera(width, height, 0.1f, 1000, 60, glm::vec3(3000.0f, 100.0f, 9000.0f));
	blocks = new Texture("Res/BlocksA.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	blocksH = new Texture("Res/BlocksH.png", GL_TEXTURE_2D, 1, GL_RGB, GL_UNSIGNED_BYTE);
	blocksN = new Texture("Res/BlocksN.png", GL_TEXTURE_2D, 2, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGB);
	Font::setUpFonts();
	Font::setScreanSize(width, height);
	shader->active();
	glm::mat4 modelMat = glm::mat4(1.0f);
	shader->setUniformMat4(modelMat, "model");
	shader->setUniformVec4(glm::vec4(1.0f,1.0f,1.0f, 1.0f), "modelColor");
	
}

Engine::~Engine()
{
	Font::freeFonts();
	Texture::clearAllTextures();
	delete camera;
	delete shader;
	delete blocks;
	delete blocksH;
	delete blocksN;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::start()
{
	float lastTime = glfwGetTime();	
	Font f("Res/ComicStans.ttf");
	Game* game = new Game(camera,window);
	std::string fps;
	std::vector<float> times;
	float changeText = 0.0;
	game->start();
	while (!glfwWindowShouldClose(window))
	{

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		changeText -= deltaTime;
		times.push_back(deltaTime);
		if (changeText <= 0)
		{
			float dt = 0;
			for (auto t : times)
				dt += t;
			dt /= times.size();
			fps = std::string(std::to_string((int)(1.0f / dt)) + " FPS\nFPS");
			changeText = 0.2137f;
			times.clear();
		}

		camera->update(window, deltaTime);
		game->update(deltaTime);
		glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		startShaderMode(*shader);
		blocks->useTexture(*shader, "tex0", 0);
		blocks->bind();
		blocksH->useTexture(*shader, "texH", 1);
		blocksH->bind();
		blocksN->useTexture(*shader, "texN", 2);
		blocksN->bind();
		camera->useCamera(*shader, "camera");
		shader->setUniformVec3(camera->getPos(), "camPos");
		shader->setUniformVec3(glm::vec3(1.0f, 1.0f, 1.0f), "lightColor");
		shader->setUniformVec2(glm::vec2(blocks->w / 512, blocks->h / 512), "textSize");
		glm::mat4 model(1.0f);
		shader->setUniformMat4(model,"model");

		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
			shader->setUniformI1(true, "debug");
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
			shader->setUniformI1(false, "debug");
		game->draw(shader);		
		endShaderMode();
		Font::setScreanSize(width, height);
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		f.drawText(fps, 0, 0, 1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		glm::vec2 pos = f.textSize(fps, 1);
		f.drawText(fps, pos.x, pos.y, 1, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		glEnable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	delete game;
}


void startShaderMode(Shader& s)
{
	s.active();
	camera->useCamera(s, "camera");
}

void endShaderMode()
{
	shader->active();
}

Shader& getDiffoltShader()
{
	return *shader;
}

Texture* getBlocks()
{
	return blocks;
}
