#include "Font.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../Properties.h"

std::vector<Font*> Font::fonts;

Font::Font()
{
	if (fonts.size() >= 1)
	{
		copyData(*fonts[0]);
		return;
	}

}

Font::Font(const char* path)
{
	for (auto f : fonts)
	{
		if (this->path.compare(path) == 0)
		{
			copyData(*f);
			return;
		}
	}
}

void Font::drawText(const char* text, int x, int y, glm::vec4 color)
{

}

void Font::copyData(Font& f)
{

}

void Font::freeFonts()
{

}

void Font::setUpFonts()
{

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
#ifdef DebugErrorMode
#define error
		printf("[Error]: Could not init FreeType Library\n");
#endif

	}
	
	FT_Face face;
	if (FT_New_Face(ft, "Res/ComicStans.ttf", 0, &face))
	{
#ifdef DebugErrorMode
#define error
		printf("[Error]: Failed to load font\n");
#endif

	}

}