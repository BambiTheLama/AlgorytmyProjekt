#include "Texture.h"
#include "Shader.h"
#include <string>
#include "Engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "../Properties.h"
#include "glm/gtc/matrix_transform.hpp"

GLuint Texture::textureSlot = 0;
std::vector<Texture*> Texture::textures;

Texture::Texture(const char* path, GLenum textureType, GLenum format, GLenum pixelType,GLenum genFormat)
{
	std::string s(path);
	if (s.size() > 4 && s.substr(s.size() - 4) == ".jpg")
	  format = GL_RGB;

	this->path = path;
	type = textureType;
	int channelsNumber;
	unsigned char* bytes = stbi_load(path, &w, &h, &channelsNumber, 0);
	glGenTextures(1, &ID);
	slot = textureSlot++;
	glActiveTexture(GL_TEXTURE0 + slot);

	glBindTexture(type, ID);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(type, 0, genFormat, w, h, 0, format, pixelType, bytes);

	glGenerateMipmap(type);

	stbi_image_free(bytes);

	glBindTexture(type, 0);
	if (w <= 0 && h <= 0)
	{
#ifdef DebugFailMode
#define info
		printf("[FAIL]: No texture loaded [%s]\n", path);
#endif
		loaded = false;
	}
	else
	{
#ifdef DebugInfoMode
#define info
		printf("[Info]: Texture loaded succesful ID: %d, [%s]\n", ID, path);
#endif

		textures.push_back(new Texture(*this));
		loaded = true;
	}

}

Texture::Texture(Texture& texture)
{
	copyData(texture);
}

Texture::~Texture()
{
}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(type, ID);
}

void Texture::unbind()
{
	glBindTexture(type, 0);
}

void Texture::useTexture(Shader& shader, const char* uniform,int n)
{
	shader.active();
	glUniform1i(shader.getUniformLocation(uniform)+n, slot);
}
void Texture::useTexture(const char* uniform) const
{
	Shader* s = getUsingShader();
	if (s)
		s->setUniformI1(slot, uniform);
}

void Texture::clearAllTextures()
{
	for (auto t : textures)
	{
#ifdef DebugInfoMode
#define info
		printf("[Info]: Texture destroyed succesful ID: %d\n", t->ID, t->path);
#endif
		glDeleteTextures(1, &t->ID);
	}
	textures.clear();
}
void Texture::copyData(Texture& texture)
{
	path = texture.path;
	type = texture.type;
	ID = texture.ID;
	h = texture.h;
	w = texture.w;
}

void Texture::deleteTexture()
{
	int i = 0;
	for (auto t : textures)
	{
		if (std::string(path).compare(t->path) == 0)
		{
#ifdef DebugInfoMode
#define info
			printf("[Info]: Texture destroyed succesful ID: %d, [%s]\n", t->ID, t->path);
#endif
			glDeleteTextures(1, &t->ID);
			break;
		}
		i++;
	}
	textures.erase(textures.begin() + i);
}



