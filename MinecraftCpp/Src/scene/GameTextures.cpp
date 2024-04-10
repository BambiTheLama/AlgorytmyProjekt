#include "GameTextures.h"
#include "../core/Texture.h"
#include "../core/Shader.h"
#include <string>
GameTextures::GameTextures(const char* path)
{
	const int n = 24;
	std::string names[n] = {
		"grass_block_top","grass_block_side","dirt","stone","snow","grass_block_snow","dirt",
		"sand","birch_log_top","birch_log","birch_leaves","birch_sapling",
		"oak_log_top","oak_log","oak_leaves","oak_sapling",
		"spruce_log_top","spruce_log","spruce_leaves","spruce_sapling",
		"cactus_top","cactus_side","cactus_top","water"
	};
	
	for (int i = 0; i < n; i++)
	{
		std::string p = std::string(path)  + names[i] + ".png";
		Texture* t = new Texture(p.c_str());
		if (!t->wasLoaded())
		{
			delete t;
			textures.push_back(NULL);
			continue;
		}
		textures.push_back(t);
	}
}

GameTextures::~GameTextures()
{
	for (auto t : textures)
	{
		delete t;
	}
	textures.clear();
}

void GameTextures::setTextures(Shader& s, const char* uniform)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (!textures[i])
			continue;
		textures[i]->useTexture(s, uniform, i);
		textures[i]->bind();
	}
}

void GameTextures::bindTextures()
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (!textures[i])
			continue;
		textures[i]->bind();
	}
}