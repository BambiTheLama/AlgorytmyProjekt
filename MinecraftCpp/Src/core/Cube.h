#pragma once
#include "Shader.h"

class Engine;

class Cube
{
	float x, y, z;
public:
	Cube(float x, float y, float z);

	void draw(Shader &shaider);

	friend class Engine;
private:
	static void CubeSetUp();
	static void CubeDelete();

};

