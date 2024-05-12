#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
class Shader;
class RenderTexture
{
	GLuint texture;
	GLuint fbo;
	GLuint rbo;
	GLenum type;
	int w, h;
	GLenum slot;

public:
	RenderTexture(int w, int h);

	~RenderTexture();

	void startUse();

	void endUse();

	void use(Shader& s, const char* uniform);

	void draw(float x = 0, float y = 0);

	friend class Engine;
private:
	static void setupRenderTextures();

	static void endRenderTexture();
};

