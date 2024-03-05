#include "Shaider.h"
#include <string>
#include <fstream>
#include <iostream>

std::string readShaider(const char* path)
{
	std::ifstream reader(path);
	std::string shaider;
	if (reader.is_open())
	{
		reader.seekg(0, std::ios::end);
		shaider.resize(reader.tellg());
		reader.seekg(0, std::ios::beg);
		reader.read(&shaider[0], shaider.size());
	}
	else
	{
		std::cout << "Cant find the shaider in the location[" << path << "]\n";
	}
	reader.close();

	return shaider;
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


Shaider::Shaider(const char* vertex, const char* frag)
{
	std::string sVertexCode = readShaider(vertex);
	std::string sFragCode = readShaider(frag);

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

Shaider::~Shaider()
{
	glDeleteProgram(ID);
}

void Shaider::active()
{
	glUseProgram(ID);
}