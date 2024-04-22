#include "GameTextures.h"
#include "../core/Texture.h"
#include "../core/Shader.h"
#include <string>
#include <fstream>
#include <json.hpp>
GameTextures::GameTextures(const char* path, const char* type)
{
	nlohmann::json j;
	std::ifstream readFile("Res/Textures.json");
	if (readFile.is_open())
	{
		readFile >> j;
	}
	readFile.close();
	if (!j.contains(type))
		return;
	const int n = j[type].size();

	
	for (int i = 0; i < n; i++)
	{
		std::string p = std::string(path) + std::string(j[type][i]) + ".png";
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
	for (int i = 0; i < textures.size() && i < 32; i++)
	{
		if (!textures[i])
			continue;
		textures[i]->useTexture(s, uniform, i);
		textures[i]->bind();
	}
	std::string u = std::string(uniform) + '2';
	const char* cu = u.c_str();
	for (int i = 0; i < textures.size()-32; i++)
	{
		if (!textures[i])
			continue;
		textures[i]->useTexture(s, cu, i);
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