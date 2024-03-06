#include "EngineCore.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image/stb_image.h"
#include "ErrorHandler.h"

EngineCore* EngineCore::engineCore = nullptr;

void EngineCore::UpdateProjectionCamera()
{
	projection = glm::mat4(1.0f);
	projection = glm::perspective<float>(glm::radians(fov), (float)width / height, zNearOrtho, zFarOrtho);
	view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

EngineCore::EngineCore()
{
	this->windowTitle = "";
	this->sceneManager = SceneManager::GetInstance(this);

	mTerrainParameters.scale = 217;
	mTerrainParameters.persistence = 0.5;
	mTerrainParameters.octaves = 4;
	mTerrainParameters.height = 110;
	mTerrainParameters.lacunarity = 1.7;
	mTerrainParameters.exponentiation = 4.5;
	mTerrainParameters.regenerateTerrain = false;
}

EngineCore::~EngineCore()
{
	delete sceneManager;
	delete engineCore;
}

EngineCore* EngineCore::GetInstance()
{
	if (engineCore == nullptr) {
		engineCore = new EngineCore();
	}
	return engineCore;
}

float EngineCore::GetDeltaTime()
{
	return deltaTime;
}

SceneManager* EngineCore::GetSceneManager()
{
	return sceneManager;
}

void EngineCore::InitWindow(const std::string& title, int width, int height)
{
	this->width = width;
	this->height = height;
	this->lastWidth = width;
	this->lastHeight = height;

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

	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW init error!" << std::endl;
	}

	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;

	glfwSetCursorPosCallback(window, OnMouse);

	GlCall(glEnable(GL_DEPTH_TEST));
	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GlCall(glEnable(GL_BLEND));

	glFrontFace(GL_CW);
	// Cull back faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	UpdateProjectionCamera();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	screenResolutionHeight = videoMode->height;
}

int EngineCore::GetWidth() const
{
	return width;
}

int EngineCore::GetHeight() const
{
	return height;
}

void EngineCore::SetPersCameraParams(float fov, float zNear, float zFar)
{
	this->fov = fov;
	this->zNear = zNear;
	this->zFar = zFar;
}

void EngineCore::GetPersCameraParams(float* fov, float* zNear, float* zFar)
{
	*fov = this->fov;
	*zNear = this->zNear;
	*zFar = this->zFar;
}

void EngineCore::SetOrthoCameraParams(float zNear, float zFar)
{
	this->zNearOrtho = zNear;
	this->zFarOrtho = zFar;
}

void EngineCore::GetOrthoCameraParams(float* zNear, float* zFar)
{
	*zNear = this->zNearOrtho;
	*zFar = this->zFarOrtho;
}

void EngineCore::OnResize(int w, int h)
{
	if (h == 0)	h = 1;
	glViewport(0, 0, w, h);
	engineCore->width = w;
	engineCore->height = h;
	engineCore->UpdateProjectionCamera();
}

void EngineCore::Deinit()
{
	glfwTerminate();
}

void EngineCore::SetOrthoCamera(bool orthoCamera)
{
	this->orthoCamera = orthoCamera;
	UpdateProjectionCamera();
}

bool EngineCore::IsOrthoCamera()
{
	return orthoCamera;
}

bool spacePressed = false;

void EngineCore::KeyInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		spacePressed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		if (spacePressed) {
			glfwSetCursorPos(window, engineCore->GetWidth() / 2, engineCore->GetHeight() / 2);
			engineCore->firstMouse = true;
			engineCore->disabledCursor = !engineCore->disabledCursor;
		}
		spacePressed = false;
	}

	float speed = 10.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		engineCore->cameraPos += speed * engineCore->cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		engineCore->cameraPos -= speed * engineCore->cameraFront;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		engineCore->cameraPos -= glm::normalize(glm::cross(engineCore->cameraFront, engineCore->cameraUp)) * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		engineCore->cameraPos += glm::normalize(glm::cross(engineCore->cameraFront, engineCore->cameraUp)) * speed;

}

void EngineCore::OnMouse(GLFWwindow* window, double xpos, double ypos)
{
	if (engineCore == nullptr || !engineCore->disabledCursor)
		return;

	if (engineCore->firstMouse)
	{
		engineCore->lastX = xpos;
		engineCore->lastY = ypos;
		engineCore->firstMouse = false;
	}

	float xoffset = xpos - engineCore->lastX;
	float yoffset = engineCore->lastY - ypos;
	engineCore->lastX = xpos;
	engineCore->lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	engineCore->yaw += xoffset;
	engineCore->pitch += yoffset;

	if (engineCore->pitch > 89.0f)
		engineCore->pitch = 89.0f;
	if (engineCore->pitch < -89.0f)
		engineCore->pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(engineCore->yaw)) * cos(glm::radians(engineCore->pitch));
	direction.y = sin(glm::radians(engineCore->pitch));
	direction.z = sin(glm::radians(engineCore->yaw)) * cos(glm::radians(engineCore->pitch));
 	engineCore->cameraFront = glm::normalize(direction);
}

glm::mat4 EngineCore::GetProjection() const
{
	return projection;
}

glm::mat4 EngineCore::GetView() const
{
	return view;
}

glm::vec3 EngineCore::GetCameraPos() const
{
	return cameraPos;
}

Renderer* EngineCore::GetRenderer()
{
	return &renderer;
}

float lastFrameTime = 0.0f;
void EngineCore::OnUpdate()
{
	sceneManager->Start();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Obliczenie czasu delta
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		KeyInput(window);

		if (engineCore->cameraPos.y > 50) {
			engineCore->cameraPos.y = 50;
		}
		else if (engineCore->cameraPos.y < -20) {
			engineCore->cameraPos.y = -20;
		}

		if (engineCore->cameraPos.x > 40) {
			engineCore->cameraPos.x = 40;
		}
		else if (engineCore->cameraPos.x < -20) {
			engineCore->cameraPos.x = -20;
		}

		if (engineCore->cameraPos.z > 40) {
			engineCore->cameraPos.z = 40;
		}
		else if (engineCore->cameraPos.z < -20) {
			engineCore->cameraPos.z = -20;
		}

		if (engineCore->disabledCursor) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		glfwGetWindowSize(window, &width, &height);
		UpdateProjectionCamera();
		glViewport(0, 0, width, height);

		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
		ImGui::Begin("Parametry terenu");
		ImGui::SetWindowCollapsed(false);
		ImGui::SetWindowSize(ImVec2(378.0f, 421.0f));
		ImGui::SetWindowPos(ImVec2(20.0f, 20.0f));
		ImGui::SetWindowFontScale(screenResolutionHeight/1080);
		sceneManager->OnDraw();

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Separator();
		if (engineCore->disabledCursor)
			ImGui::Text("Tryb podgladu (spacja).");
		else
			ImGui::Text("Tryb edycji (spacja).");
		
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	sceneManager->Deinit();

	//Deinit();
}

TerrainParameters* EngineCore::GetTerrainParameters()
{
	return &mTerrainParameters;
}