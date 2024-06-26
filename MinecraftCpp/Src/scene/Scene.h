#pragma once
class Shader;
class Scene
{
public:
	virtual ~Scene() {};

	virtual void update(float deltaTime) = 0;

	virtual void draw() = 0;
};