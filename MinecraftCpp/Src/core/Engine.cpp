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
#include "RenderTexture.h"

static Shader* shader = NULL;
static Shader* shaderShadow = NULL;
static Shader* textShader = NULL;
static Camera* camera = NULL;
static Texture* blocks = NULL;
static Texture* blocksH = NULL;
static Texture* blocksN = NULL;
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

	shaderShadow = new Shader("Shader/Shadow.vert", "Shader/Shadow.frag");
	camera = new Camera(width, height, 0.1f, 1000, 60, glm::vec3(3000.0f, 100.0f, 9000.0f));
	blocks = new Texture("Res/Blocks64.png", GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE);
	blocksH = new Texture("Res/Blocks64H.png", GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE);
	blocksN = new Texture("Res/Blocks64N.png", GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGB);
	Font::setUpFonts();
	Font::setScreanSize(width, height);
	shader->active();
	glm::mat4 modelMat = glm::mat4(1.0f);
	shader->setUniformMat4(modelMat, "model");
	shader->setUniformVec4(glm::vec4(1.0f,1.0f,1.0f, 1.0f), "modelColor");
	RenderTexture::setUpRenderTextures();

	shader->active();






	e = this;


}

Engine::~Engine()
{
	RenderTexture::endRenderTexture();
	Font::freeFonts();
	Texture::clearAllTextures();
	delete camera;
	delete shader;
	delete blocks;
	delete blocksH;
	delete blocksN;
	delete shaderShadow;
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
	Game* game = new Game(camera,window);
	std::string fps;
	std::vector<float> times;
	float changeText = 0.0;
	game->start();
	RenderTexture* rt = new RenderTexture(2048, 2048);
	rt->use(*shader, "texShadow");
	glm::vec3 cameraDir = camera->getDir();
	glm::vec3 cameraPos = camera->getDir();
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


		camera->update(window, deltaTime);


		game->update(deltaTime);
		glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		startShaderMode(*shader);

		camera->useCamera(*shader, "camera");
		shader->setUniformVec3(camera->getPos(), "camPos");
		shader->setUniformVec3(glm::vec3(1.0f, 1.0f, 1.0f), "lightColor");
		shader->setUniformVec2(glm::vec2(blocks->w / 64, blocks->h / 64), "textSize");
		glm::mat4 model(1.0f);
		shader->setUniformMat4(model,"model");

		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		rt->startUse();

		cameraDir = camera->getDir();
		cameraPos = camera->getPos();
		glm::vec3 lightDir = glm::vec3(0.5f, -1.0f,sin(time)*0.0f+0.6f);
		camera->setDir(lightDir);

		camera->setUseProjection(false);
		camera->newPos(glm::vec3(((int)cameraPos.x/16)*16, 255, ((int)cameraPos.z / 16) * 16));

		shaderShadow->active();
		camera->useCamera(*shaderShadow, "camera");
		game->draw(shaderShadow);
		rt->endUse();
		shader->active();
		camera->useCamera(*shader, "lightProjection");
		shader->setUniformVec3(lightDir, "lightDir");
		rt->use(*shader, "texShadow");
		blocks->useTexture(*shader, "tex0");
		blocksH->useTexture(*shader, "texH");
		blocksN->useTexture(*shader, "texN");
		blocks->bind();
		blocksH->bind();
		blocksN->bind();
		
		camera->setDir(cameraDir);
		camera->newPos(cameraPos);
		camera->setUseProjection(true);
		camera->useCamera(*shader, "camera");
		game->draw(shader);

		rt->draw();
		endShaderMode();
		Font::setScreanSize(width, height);
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);
		f.drawText(fps, 0, 0, 1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		glm::vec2 pos = f.textSize(fps, 1);
		f.drawText(fps, pos.x, pos.y, 1, glm::vec4(1.0f, 0.79f, 0.2137f, 1.0f));
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
