#pragma once
//#include <ft2build.h>
//#include FT_FREETYPE_H

#include <glm/vec4.hpp>
#include <vector>
#include <string>
#include <map>
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Character {
	unsigned int TextureID;
	glm::ivec2   Size;     
	glm::ivec2   Bearing;  
	unsigned int Advance;  
};

class Font
{
	std::string path;
	std::map<GLchar, Character> Characters;
public:
	Font();

	Font(const char* path);

	~Font();

	void drawText(std::string text, int x, int y, int size, glm::vec4 color);

	friend class Engine;

private:

	static void freeFonts();

	static void setScreanSize(float w, float h);

	static void setUpFonts();
};

