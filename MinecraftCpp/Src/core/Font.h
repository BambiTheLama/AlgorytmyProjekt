#pragma once
//#include <ft2build.h>
//#include FT_FREETYPE_H

#include <glm/vec4.hpp>
#include <vector>
#include <string>
class Font
{
	static std::vector<Font*> fonts;
	std::string path;
public:
	Font();

	Font(const char* path);

	void drawText(const char* text, int x, int y, glm::vec4 color);

	friend class Engine;

private:
	void copyData(Font& f);

	static void freeFonts();

	static void setUpFonts();
};

