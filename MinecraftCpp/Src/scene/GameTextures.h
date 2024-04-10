#pragma once
#include <vector>
class Texture;
class Shader;
class GameTextures
{
	std::vector<Texture*> textures;
public:
	GameTextures(const char* path);

	~GameTextures();

	void setTextures(Shader& s, const char* uniform);

	void bindTextures();
};

