#pragma once
#include <time.h>
#include <stdlib.h>

class PerlinNoice
{
	int h, w;
	float scalling = 1.0f;
	float** seed2D;
	float** noise;
	float frq=0.3f;
	int octavies = 8;
public:

	PerlinNoice(int seed=2137);

	PerlinNoice(int w, int h, int seed = 2137);

	void newSeed(int seed);

	~PerlinNoice();

	void generateNoice2D();

	float** getNoice2D() { return noise; }

	float getNoise(int x, int y);

	int getH() { return h; }

	int getW() { return w; }

	void addToScalling(float a) { scalling += a; }

	float getScalling() { return scalling; }
};

