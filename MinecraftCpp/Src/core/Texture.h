#pragma once
#include <glad/glad.h>
#include <vector>
class Shader;

class Texture
{
	GLuint ID;
	GLenum type;
	int w, h;
	const char* path;
	static std::vector<Texture*> textures;
public:

	Texture(const char* path, GLenum textureType = GL_TEXTURE_2D, GLenum slot = GL_TEXTURE0, GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE);

	Texture(Texture& texture);

	~Texture();

	void bind();

	void unbind();

	void useTexture(Shader& shader, const char* uniform, GLuint unit);

	int getW() const { return w; }

	int getH() const { return h; }

	void deleteTexture();

	friend class Engine;
private:
	static void clearAllTextures();

	void copyData(Texture& texture);


};

