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
static Shader* shader = NULL;
static Camera* camera = NULL;
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
	stbi_set_flip_vertically_on_load(true);

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

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	Cube::CubeSetUp();
	vao = new VAO();
	vao->bind();
	vbo = new VBO(verticies, sizeof(verticies), GL_DYNAMIC_DRAW);
	ebo = new EBO(indices, sizeof(indices));
	vao->linkData(*vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	vbo->unbind();
	ebo->unbind();
	vao->unbind();
	shader = new Shader("Shader/Dif.vert", "Shader/Dif.frag");
	camera = new Camera(width, height, 0.1f, 100, 100, glm::vec3(0.0f, 0.0f, 0.0f));
}

Engine::~Engine()
{
	delete camera;
	delete shader;
	delete vbo;
	delete vao;
	delete ebo;
	Cube::CubeDelete();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Engine::start()
{
	float lastTime = glfwGetTime();	
	Texture text("Res/1.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

	Cube*** cub = NULL;
	int w = 3, h = 2;
	cub = new Cube * *[h];
	for (int y = 0; y < h; y++)
	{
		cub[y] = new Cube * [w];
		for (int x = 0; x < w; x++)
		{
			cub[y][x] = new Cube(-w / 2.0f + x*3, 0, -h / 2.0f + y*3);
			if (x >= 0)
			{
				//cub[y][x]->left = false;
				//cub[y][x]->right = false;
				//cub[y][x]->front = false;
				//cub[y][x]->back = false;
			}
		}
	}

	Cube c(0, 0, 1);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		glfwSetWindowTitle(window, std::string(title + " :" + std::to_string(1.0f / deltaTime)+" FPS").c_str());
		camera->update(window, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->updatePos(glm::vec3(0.0f,0.0f,1.0f*deltaTime));
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->updatePos(glm::vec3(0.0f, 0.0f, -1.0f * deltaTime));
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->updatePos(glm::vec3(1.0f * deltaTime, 0.0f, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->updatePos(glm::vec3(-1.0f * deltaTime, 0.0f, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera->updatePos(glm::vec3(0.0f, 1.0f * deltaTime, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			camera->updatePos(glm::vec3(0.0f, -1.0f * deltaTime, 0.0f));

		glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader->active();
		text.bind();
		text.useTexture(*shader, "tex0", 0);
		startShaderMode(*Cube::shader);
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				cub[y][x]->draw();
			}
		}
		endShaderMode();
		camera->draw(*shader);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			delete cub[y][x];
		}
		delete cub[y];
	}
	delete cub;
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