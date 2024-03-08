#include "Texture.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const char* path, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType)
{
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
		printf("NIE MA TEKSTURY\n");
}
Texture::~Texture()
{
	glDeleteTextures(1, &ID);
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
