#include "Texture.h"
#include "Shader.h"
#include <string>
#include "Engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::vector<Texture*> Texture::textures;

Texture::Texture(const char* path, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType)
{
	for (auto t : textures)
	{
		if (std::string(path).compare(t->path) == 0)
		{
			copyData(*t);
			return;
		}

	}
	this->path = path;
	type = textureType;
	int numColCh;
	unsigned char* bytes = stbi_load(path, &w, &h, &numColCh, 0);
	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(type, ID);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(type, 0, GL_RGBA, w, h, 0, format, pixelType, bytes);

	glGenerateMipmap(type);

	stbi_image_free(bytes);

	glBindTexture(type, 0);
	if (w <= 0 && h <= 0)
	{
		printf("[FAIL]: No texture loaded [%s]\n", path);
	}
	else
	{
		printf("[Info]: Texture loaded succesful ID: %d, [%s]\n", ID, path);
		textures.push_back(new Texture(*this));
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
	glBindTexture(type, ID);
}

void Texture::unbind()
{
	glBindTexture(type, 0);
}

void Texture::useTexture(Shader& shader, const char* uniform, GLuint unit)
{
	shader.active();
	glUniform1i(shader.getUniformLocation(uniform), unit);
}
void Texture::useTexture(const char* uniform, GLuint unit)
{
	Shader& s = getDiffoltShader();
	//s.active();
	glUniform1i(s.getUniformLocation(uniform), unit);
}
void Texture::clearAllTextures()
{
	for (auto t : textures)
	{
		printf("[Info]: Texture destroyed succesful ID: %d, [%s]\n", t->ID, t->path);
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
			printf("[Info]: Texture destroyed succesful ID: %d, [%s]\n", t->ID, t->path);
			glDeleteTextures(1, &t->ID);
			break;
		}
		i++;
	}
	textures.erase(textures.begin() + i);
}