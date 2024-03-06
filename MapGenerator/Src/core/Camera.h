#pragma once
#include <glm/matrix.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Shader.h"
class Camera
{
	glm::vec3 cameraPos;
	glm::vec3 cameraOrientation = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float cameraWidth;
	float cameraHeight;
	float farest;
	float nearest;
	float cameraAngleDeg = 90;
	bool useProjection = true;
public:
	Camera(float width, float height, float farest, float neares,float cameraAngle=45.0f, glm::vec3 position = glm::vec3(0));

	void useCamera(Shader& shader,const char* uniform);

	void updatePos(glm::vec3 pos);

	void newPos(glm::vec3 pos);

	void setUseProjection(bool use) { useProjection = use; }

};

