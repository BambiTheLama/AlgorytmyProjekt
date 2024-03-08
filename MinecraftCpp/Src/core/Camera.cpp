#include "Camera.h"
#include "Engine.h"
#include "Shader.h"

Camera::Camera(float width, float height, float farest, float neares, float cameraAngle, glm::vec3 position)
{
	cameraHeight = height;
	cameraWidth = width;
	cameraPos = position;
	this->farest = farest;
	this->nearest = neares;
	this->cameraAngleDeg = cameraAngle;
}

void Camera::useCamera(Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraPos + cameraOrientation, up);

	if (useProjection)
		projection = glm::perspective(glm::radians(cameraAngleDeg), cameraWidth / cameraHeight, nearest, farest);
	else
		projection = glm::ortho(0.0f, cameraWidth, cameraHeight, 0.0f);
	glUniformMatrix4fv(shader.getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::updatePos(glm::vec3 pos)
{
	cameraPos += pos;
}

void Camera::newPos(glm::vec3 pos)
{
	cameraPos = pos;
}

void Camera::draw(Shader& shader)
{
	glm::vec3 cameraPos = this->cameraPos + cameraOrientation;
	drawTriangle(cameraPos + glm::vec3(0, 0, 0), cameraPos + glm::vec3(0.02f, 0.01f, 0), cameraPos + glm::vec3(-0.02f, 0.01f, 0));
	drawTriangle(cameraPos + glm::vec3(0, 0, 0), cameraPos - glm::vec3(0.02f, 0.01f, 0), cameraPos - glm::vec3(-0.02f, 0.01f, 0));
}