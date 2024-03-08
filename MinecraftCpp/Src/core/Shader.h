#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shader
{
	GLuint ID;


public:
	Shader(const char* vertex, const char* frag);

	~Shader();

	void active();

	GLint getUniformLocation(const char* uniform);

	void setUniformMat4(glm::mat4 mat,const char* uniform);

	void setUniformVec4(glm::vec4 mat, const char* uniform);

	void setUniformVec3(glm::vec3 vec, const char* uniform);

	void setUniformI1(int v, const char* uniform);
};

