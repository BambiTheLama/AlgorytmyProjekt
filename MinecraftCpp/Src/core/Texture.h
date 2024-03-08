#pragma once
#include <glad/glad.h>
class Shader;

class Texture
{
	GLuint ID;
	GLenum type;
	int w, h;
public:

	Texture(const char* path, GLenum textureType = GL_TEXTURE_2D, GLenum slot = GL_TEXTURE0, GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE);

	~Texture();

	void bind();

	void unbind();

	void useTexture(Shader& shader, const char* uniform, GLuint unit);

	int getW() const { return w; }

	int getH() const { return h; }
};

