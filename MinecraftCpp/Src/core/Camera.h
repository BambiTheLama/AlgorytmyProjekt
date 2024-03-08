#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <glm/glm.hpp>


class Shader;
class Camera
{
	glm::vec3 cameraPos;
	glm::vec3 cameraOrientation = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	bool click = true;
	float cameraSensivity = 100;
	float cameraWidth;
	float cameraHeight;
	float farest;
	float nearest;
	float speed = 5;
	float cameraAngleDeg = 90;
	bool useProjection = true;
public:
	Camera(float width, float height, float farest, float neares,float cameraAngle=45.0f, glm::vec3 position = glm::vec3(0));

	void useCamera(Shader& shader,const char* uniform);

	void updatePos(glm::vec3 pos);

	void newPos(glm::vec3 pos);

	void setUseProjection(bool use) { useProjection = use; }

	void draw(Shader& shader);

	void update(GLFWwindow* window,float deltaTime);

};

