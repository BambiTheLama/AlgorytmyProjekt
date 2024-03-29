#include "Font.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../Properties.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static glm::mat4 projection;
static Shader* shader = NULL;
static unsigned int VAO, VBO;

Font::Font():Font("Res/ComicStans.ttf")
{


}

Font::Font(const char* path)
{
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }


    std::string font_name = "Res/ComicStans.ttf";
    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 64);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 255; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
Font::~Font()
{

    for (auto c : Characters)
    {
        glDeleteTextures(1, &c.second.TextureID);
    }

}

int getValueFromText(std::string text, int begin, int end)
{
    int v = 0;
    for (int i = begin; i < end && i < text.size(); i++)
    {
        if (text[i] >= '0' && text[i] <= '9')
        {
            v = v * 10 + text[i] - '0';
        }
        if (text[i] == ',' || text[i] == '.' || text[i] == ' ' || text[i] == '}')
            return v;
    }
    return v;
}
int getValueStartPos(std::string text, int begin, int end)
{
    for (int i = begin; i < end && i < text.size(); i++)
    {
        if (text[i] >= '0' && text[i] <= '9')
        {
            return i;
        }
    }
    return -1;
}
int getSeparatorStartPos(std::string text, int begin, int end)
{
    for (int i = begin; i < end && i < text.size(); i++)
    {
        if (text[i] == ',' || text[i] == '.')
        {
            return i;
        }
    }
    return -1;
}
void Font::drawText(std::string text, int x, int y,int size, glm::vec4 color)
{
    shader->active();
    shader->setUniformMat4(projection, "projection");
    shader->setUniformVec4(color, "textColor");

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    int dx = x;

    for (int i = 0; i < text.size(); i++)
    {

        Character ch = Characters[text[i]];
        if (text[i] == '\n')
        {
            dx = x;
            y -= ch.Size.y * size * 1.5f;
            continue;
        }
        if (text[i] == '{')
        {
            int end = -1;
            for (int j = i + 1; j < text.size(); j++)
            {
                if (text[j] == '}')
                {
                    end = j;
                    break;
                }
            }
            if (text[i + 1] == 'c')
            {
                int startPos = getValueStartPos(text, i, end);
                int seperatorPos = getSeparatorStartPos(text, i, end);
                int r = getValueFromText(text, startPos, end);
                startPos = getValueStartPos(text, seperatorPos, end);
                seperatorPos = getSeparatorStartPos(text, startPos, end);
                int g = getValueFromText(text, startPos, end);
                startPos = getValueStartPos(text, seperatorPos, end);
                seperatorPos = getSeparatorStartPos(text, startPos, end);
                int b = getValueFromText(text, startPos, end);
                startPos = getValueStartPos(text, seperatorPos, end);
                seperatorPos = getSeparatorStartPos(text, startPos, end);
                int a = getValueFromText(text, startPos, end);
                if (a <= 0)
                    a = 255;
                color.r = r / 255.f;
                color.g = g / 255.f;
                color.b = b / 255.f;
                color.a = a / 255.0f;
                shader->setUniformVec4(color, "textColor");
                i = end;
                continue;
            }
        }

        float xpos = dx + ch.Bearing.x * size;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * size;

        float w = ch.Size.x * size;
        float h = ch.Size.y * size;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        dx += (ch.Advance >> 6) * size; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Font::freeFonts()
{
    if (shader)
        delete shader;
    shader = NULL;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Font::setUpFonts()
{
    shader = new Shader("Shader/text.vert", "Shader/text.frag");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Font::setScreanSize(float w, float h)
{
    if (!shader)
        return;
    projection = glm::ortho(0.0f, w, 0.0f, h, -1.0f, 1.0f);
}