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

static GLfloat verticies[] = {
	1,1,1,
	1,-1,1,
	-1,1,1,
};

static GLuint indices[] = {
	0,2,1,
	0,1,2,

};
static VAO* vao = NULL;
static VBO* vbo = NULL;
static EBO* ebo = NULL;
static GLuint textVAO;
static GLuint textVBO;
static Shader* shader = NULL;
static Shader* textShader = NULL;
static Camera* camera = NULL;
static Texture* blocks = NULL;
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

	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vao = new VAO();
	vao->bind();
	vbo = new VBO(verticies, sizeof(verticies), GL_DYNAMIC_DRAW);
	ebo = new EBO(indices, sizeof(indices));
	vao->linkData(*vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	vbo->unbind();
	ebo->unbind();
	vao->unbind();
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader = new Shader("Shader/Diff.vert", "Shader/Diff.geom", "Shader/Diff.frag");
	camera = new Camera(width, height, 0.1f, 300, 60, glm::vec3(0.0f, 100.0f, 0.0f));
	blocks = new Texture("Res/Blocks.png");
	Font::setUpFonts();
	
}

Engine::~Engine()
{
	Texture::clearAllTextures();
	delete camera;
	delete shader;
	delete vbo;
	delete vao;
	delete ebo;
	delete blocks;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::start()
{
	float lastTime = glfwGetTime();	
	
	Game* game = new Game(camera,window);

	game->start();
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		glfwSetWindowTitle(window, std::string(title + " :" + std::to_string(1.0f / deltaTime)+" FPS").c_str());
		camera->update(window, deltaTime);
		game->update(deltaTime);
		glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		startShaderMode(*shader);
		blocks->useTexture("tex0", 0);

		shader->setUniformVec2(glm::vec2( 4, 5), "textSize");
		blocks->bind();
		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
			shader->setUniformI1(true, "debug");
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
			shader->setUniformI1(false, "debug");
		game->draw();
		game->endPhase();
		
		camera->draw(*shader);
		endShaderMode();
		glfwSwapBuffers(window);

		glfwPollEvents();

	}
	delete game;
}

void drawTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,glm::vec3 color)
{

	verticies[0] = p1.x;
	verticies[1] = p1.y;
	verticies[2] = p1.z;
	verticies[3] = p2.x;
	verticies[4] = p2.y;
	verticies[5] = p2.z;
	verticies[6] = p3.x;
	verticies[7] = p3.y;
	verticies[8] = p3.z;
	vbo->setNewVertices(verticies, sizeof(verticies));
	vao->bind();
	vbo->bind();
	ebo->bind();
	glm::mat4 modelMat = glm::mat4(1.0f);
	shader->setUniformMat4(modelMat, "model");
	shader->setUniformVec4(glm::vec4(color, 1.0f), "modelColor");
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
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
