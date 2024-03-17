#include "camera.h"
#include "Engine.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

Camera::Camera(float width, float height, float neares, float farest, float cameraAngle, glm::vec3 position)
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
		projection = glm::ortho(0.0f, cameraWidth, 0.0f, cameraHeight);
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
	glm::vec3 right = glm::normalize(glm::cross(up, cameraOrientation));
	shader.setUniformVec3(glm::vec3(0.0f), "pos");

}

void Camera::update(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += speed * cameraOrientation * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos += speed * deltaTime * -glm::normalize(glm::cross(cameraOrientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos += speed * deltaTime * -cameraOrientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += speed * deltaTime * glm::normalize(glm::cross(cameraOrientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cameraPos += speed * deltaTime * up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		cameraPos += speed * deltaTime * -up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = maxSpeed;
	}
	else
	{
		speed = minSpeed;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (click)
		{
			glfwSetCursorPos(window, (cameraWidth / 2), (cameraHeight / 2));
			click = false;
		}

		double mouseX;
		double mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = cameraSensivity * (float)(mouseY - (cameraHeight / 2)) / cameraHeight;
		float rotY = cameraSensivity * (float)(mouseX - (cameraWidth / 2)) / cameraWidth;

		glm::vec3 newcameraOrientation = glm::rotate(cameraOrientation, glm::radians(-rotX), glm::normalize(glm::cross(cameraOrientation, up)));

		if (abs(glm::angle(newcameraOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			cameraOrientation = newcameraOrientation;
		}

		cameraOrientation = glm::rotate(cameraOrientation, glm::radians(-rotY), up);

		glfwSetCursorPos(window, (cameraWidth / 2), (cameraHeight / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		click = true;
	}
}