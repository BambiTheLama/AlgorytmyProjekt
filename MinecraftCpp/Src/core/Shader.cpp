#include "Shader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

std::string readShader(const char* path)
{
	std::ifstream reader(path);
	std::string Shader;
	if (reader.is_open())
	{
		reader.seekg(0, std::ios::end);
		Shader.resize(reader.tellg());
		reader.seekg(0, std::ios::beg);
		reader.read(&Shader[0], Shader.size());
	}
	else
	{
		std::cout << "Cant find the Shader in the location[" << path << "]\n";
	}
	reader.close();

	return Shader;
}

void compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}


Shader::Shader(const char* vertex, const char* frag)
{
	std::string sVertexCode = readShader(vertex);
	std::string sFragCode = readShader(frag);

	const char* vertexCode = sVertexCode.c_str();
	const char* fragCode = sFragCode.c_str();

	GLuint vertID = glCreateShader(GL_VERTEX_SHADER);;

	glShaderSource(vertID, 1, &vertexCode, NULL);
	glCompileShader(vertID);
	compileErrors(vertID, "VERTEX");

	GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragID, 1, &fragCode, NULL);
	glCompileShader(fragID);
	compileErrors(fragID, "FRAGMENT");

	ID = glCreateProgram();

	glAttachShader(ID, vertID);
	glAttachShader(ID, fragID);
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	glDeleteShader(fragID);
	glDeleteShader(vertID);

}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::active()
{
	glUseProgram(ID);
}

GLint Shader::getUniformLocation(const char* uniform)
{
	return glGetUniformLocation(ID, uniform);
}
void Shader::setUniformMat4(glm::mat4 mat, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, uniform), 1, GL_FALSE, glm::value_ptr(mat));
}
void Shader::setUniformVec4(glm::vec4 vec, const char* uniform)
{
	glUniform4f(glGetUniformLocation(ID, uniform), vec.x, vec.y, vec.z, vec.w);
}
void Shader::setUniformVec3(glm::vec3 vec, const char* uniform)
{
	glUniform3f(glGetUniformLocation(ID, uniform), vec.x, vec.y, vec.z);
}
void Shader::setUniformI1(int v, const char* uniform)
{
	glUniform1i(glGetUniformLocation(ID, uniform), v);
}

