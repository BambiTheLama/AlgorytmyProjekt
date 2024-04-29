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


static Engine* e;
static Shader* shader = NULL;
static VAO* vao = NULL;
static VBO* vbo = NULL;
static EBO* ebo = NULL;
static std::vector<GLuint> indices{
	//BACK
	0,2,1,
	3,2,0,
	//FRONT
	4,5,6,
	6,7,4,
	//LEFT
	0,1,4,
	1,5,4,
	//RIGHT
	2,3,6,
	3,7,6,
	//DOWN
	0,4,3,
	3,4,7,
	//UP
	1,2,5,
	2,6,5,

};
static std::vector<glm::vec3> verticesPos{
	glm::vec3(0,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,1,1),
	glm::vec3(0,0,1),
	glm::vec3(1,0,0),
	glm::vec3(1,1,0),
	glm::vec3(1,1,1),
	glm::vec3(1,0,1),

};

void resize(GLFWwindow* window, int width, int height)
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
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSwapInterval(0);
	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Font::setupFonts();
	Font::setScreenSize(width, height);

	RenderTexture::setupRenderTextures();


	shader = new Shader("Shader/Cursor.vert", "Shader/Cursor.frag");
	vao = new VAO();
	vbo = new VBO();
	ebo = new EBO();
	vbo->setNewVertices(verticesPos);
	vao->bind();
	ebo->setNewIndices(indices);
	vao->linkData(*vbo, 0, 3, GL_FLOAT, sizeof(glm::vec3), NULL);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	e = this;
	start();
}

Engine::~Engine()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	RenderTexture::endRenderTexture();
	Font::freeFonts();
	Texture::clearAllTextures();

	glfwDestroyWindow(window);
	glfwTerminate();
	e = NULL;
	delete shader;
	delete vao;
	delete vbo;
}

void diffViewport()
{
	glViewport(0, 0, e->width, e->height);
}

void Engine::start()
{
	float lastTime = glfwGetTime();	
	Font f("Res/ComicStans.ttf");
	Game* game = new Game(width, height, window,io);
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
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		game->draw();



		Font::setScreenSize(width, height);
		glDisable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);
		f.drawText(fps, 0, 0, 1, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		glEnable(GL_DEPTH_TEST);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete game;
}

void drawCubeAt(int x, int y, int z,Camera *camera,char faces)
{
	std::vector<GLuint> ind;
	if ((faces&(char)Faces::Back)>0)
	{
		for (int i = 0; i < 6; i++)
			ind.push_back(indices[i+12]);
	}
	if ((faces & (char)Faces::Front) > 0)
	{
		for (int i = 0; i < 6; i++)
			ind.push_back(indices[i + 18]);
	}
	if ((faces & (char)Faces::Up) > 0)
	{
		for (int i = 0; i < 6; i++)
			ind.push_back(indices[i + 30]);
	}
	if ((faces & (char)Faces::Down) > 0)
	{
		for (int i = 0; i < 6; i++)
			ind.push_back(indices[i + 24]);
	}
	if ((faces & (char)Faces::Right) > 0)
	{
		for (int i = 0; i < 6; i++)
			ind.push_back(indices[i + 6]);
	}
	if ((faces & (char)Faces::Left) > 0)
	{
		for (int i = 0; i < 6; i++)
			ind.push_back(indices[i]);
	}
	shader->active();
	vao->bind();
	ebo->bind();
	ebo->setNewIndices(ind);
	camera->useCamera(*shader, "camera");
	shader->setUniformVec3(glm::vec3(x, y, z), "cubePos");
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthFunc(GL_EQUAL);
	glDrawElements(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}