#include "PerlinNoise.h"
#include <math.h>

PerlinNoice::PerlinNoice(int seed)
{
	h = 6400;
	w = 6400;
	noise = new float* [h];
	for (int i = 0; i < h; i++)
		noise[i] = new float[w];

	seed2D = new float* [h];
	for (int i = 0; i < h; i++)
		seed2D[i] = new float[w];

	newSeed(seed);
	generateNoice2D();
}

PerlinNoice::PerlinNoice(int w, int h, int seed)
{

	this->h = h;
	this->w = w;

	noise = new float* [h];
	for (int i = 0; i < h; i++)
		noise[i] = new float[w];

	seed2D = new float* [h];
	for (int i = 0; i < h; i++)
		seed2D[i] = new float[w];

	newSeed(seed);
	generateNoice2D();
}

void PerlinNoice::newSeed(int seed)
{
	srand(seed);
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			seed2D[i][j] = (float)rand() / (float)RAND_MAX;

}

PerlinNoice::~PerlinNoice()
{
	for (int i = 0; i < h; i++)
	{
		delete seed2D[i];
		delete noise[i];
	}
	delete seed2D;
	delete noise;
}

void PerlinNoice::generateNoice2D()
{
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			float noice = 0.0f;
			float scale = 1.0f;
			float scaleAcc = 0.0f;
			for (int k = 0; k < octavies; k++)
			{
				int level1 = w >> octavies;
				int level2 = h >> octavies;
				if (level1 == 0)
					level1 = 1;
				if (level2 == 0)
					level2 = 1;
				int x1 = (x / level1) * level1;
				int y1 = (y / level2) * level2;

				int x2 = (x1 + level1) % w;
				int y2 = (y1 + level2) % h;

				float blend1 = (float)(x - x1) / (float)level1;
				float blend2 = (float)(y - y1) / (float)level2;

				float sample1 = (1.0f - blend1) * seed2D[y1][x1] + blend1 * seed2D[y1][x2];
				float sample2 = (1.0f - blend1) * seed2D[y2][x1] + blend1 * seed2D[y2][x2];

				scaleAcc += scale;
				noice += (blend2 * (sample2 - sample1) + sample1) * scale;
				scale /= scalling;
			}
			noise[y][x] = noice / scaleAcc;
		}

}

float PerlinNoice::getNoise(int x, int y) 
{
	int sx = (int)(frq * abs(x)) % w;
	int sy = (int)(frq * abs(y)) % h;
	int ex = (sx + 1) % w;
	int ey = (sy + 1) % h;
	float pX = (frq * abs(x)) - (int)(frq * abs(x));
	float pY = (frq * abs(y)) - (int)(frq * abs(y));
	float downV = noise[sy][sx] * (1.0f - pX) + noise[sy][ex] * (pX);
	float upV = noise[ey][sx] * (1.0f - pX) + noise[ey][ex] * (pX);

	return downV * (1.0f - pY) + upV * pY;
}