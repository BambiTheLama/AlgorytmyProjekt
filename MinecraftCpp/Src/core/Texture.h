#pragma once
#include <glad/glad.h>
#include <vector>
class Shader;

class Texture
{
	static GLuint textureSlot;
	GLuint ID;
	GLenum type;
	int w, h;
	const char* path;
	bool loaded = false;
	GLenum slot;
	static std::vector<Texture*> textures;
public:

	Texture(const char* path, GLenum textureType = GL_TEXTURE_2D,
		GLenum format = GL_RGBA, GLenum pixelType = GL_UNSIGNED_BYTE, GLenum genFormat = GL_RGBA);

	Texture(Texture& texture);

	~Texture();

	void bind();

	void unbind();

	void useTexture(Shader& shader, const char* uniform, int n = 0);

	void useTexture(const char* uniform);

	void setTextureSize(const char* uniform);

	int getW() const { return w; }

	int getH() const { return h; }

	void deleteTexture();

	bool wasLoaded() { return loaded; }

	friend class Engine;
	friend class RenderTexture;
private:
	static void clearAllTextures();

	void copyData(Texture& texture);



};

