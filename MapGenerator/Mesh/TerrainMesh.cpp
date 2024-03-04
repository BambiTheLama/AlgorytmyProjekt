#include "TerrainMesh.h"

#include <cmath>
#include <iostream>

#include "../Core/ImageManager.h"

TerrainMesh::TerrainMesh(TerrainParameters* params)
{
    mParams = params;
    noiseGen = new SimplexNoise(false);
    heightMapData = new unsigned char[WIDTH * HEIGHT];
    GenerateTerrain();

    SetMesh(verticesNormals, indices);
}

TerrainMesh::~TerrainMesh()
{
    delete[] heightMapData;
    delete noiseGen;
}

void TerrainMesh::SetGenerateDataFromImage()
{
    regenerateDataFromImage = true;
}

void TerrainMesh::GenerateTerrainMesh()
{
    GenerateTerrain();

    UpdateVerticesMesh(verticesNormals);
}

void TerrainMesh::UpdateTerrainParameters(TerrainParameters* params)
{
    mParams = params;
    GenerateTerrain();
}

void TerrainMesh::SaveTerrainDataToImage(const char* filename)
{
    ImageManager::SaveImage(filename, WIDTH, HEIGHT, heightMapData);
}

unsigned char* TerrainMesh::GetNoiseSeedArray()
{
    return noiseGen->GetPermTable();
}

unsigned char* TerrainMesh::GetHeightMapDataArray()
{
    return heightMapData;
}

float TerrainMesh::Get(int x, int z) {
    /*float scale = 217;
    float persistence = 0.5;
    int octaves = 4;
    int height = 110;
    float lacunarity = 1.7;
    float exponentiation = 4.5;*/
    float scale = mParams->scale;
    float persistence = mParams->persistence;
    int octaves = mParams->octaves;
    int height = mParams->height;
    float lacunarity = mParams->lacunarity;
    float exponentiation = mParams->exponentiation;
    if (mParams->regenerateTerrain && !regenerateDataFromImage) {
        noiseGen->RandPermArray();
        mParams->regenerateTerrain = false;
    }

    noiseGen->SetParams(1.0f, 1.0f, lacunarity, persistence);

    float xs = x / scale;
    float ys = z / scale;
    float G = pow(2.0f, -persistence);
    float amplitude = 1.0;
    float frequency = 1.0;
    float normalization = 0;
    float total = 0;
    for (int o = 0; o < octaves; o++) {
        float noiseValue = noiseGen->fractal(octaves,
            xs * frequency, ys * frequency) * 0.5 + 0.5;

        total += noiseValue * amplitude;
        normalization += amplitude;
        amplitude *= G;
        frequency *= lacunarity;
    }
    total /= normalization;
    return pow(total, exponentiation) * height;
}

// Funkcja wykonuj¹ca interpolacjê dwuliniow¹
float interpolateBilinear(const unsigned char* heightMap, int x, int y)
{
    int x0 = x;
    int y0 = y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float dx = x - static_cast<float>(x0);
    float dy = y - static_cast<float>(y0);

    // Sprawdzenie, czy punkt znajduje siê w granicach mapy wysokoœci
    if (x0 >= 0 && x1 < 256 && y0 >= 0 && y1 < 256)
    {
        float q00 = heightMap[y0 * 255 + x0];
        float q01 = heightMap[y1 * 255 + x0];
        float q10 = heightMap[y0 * 255 + x1];
        float q11 = heightMap[y1 * 255 + x1];

        // Interpolacja wzd³u¿ osi X
        float h0 = q00 * (1.0f - dx) + q10 * dx;
        float h1 = q01 * (1.0f - dx) + q11 * dx;

        // Interpolacja wzd³u¿ osi Y
        float h = h0 * (1.0f - dy) + h1 * dy;

        return h;
    }

    // Zwracanie domyœlnej wartoœci, jeœli punkt znajduje siê poza granicami mapy wysokoœci
    return 0.0f;
}

float InterpolateHeight(const unsigned char* heightMap, int x, int y)
{
    int xFloor = x;
    int yFloor = y;

    float u = x - xFloor;
    float v = y - yFloor;

    // Ensure the coordinates are within the valid range
    if (xFloor < 0 || xFloor >= 256 - 1 || yFloor < 0 || yFloor >= 256 - 1)
    {
        return 0.0f;
    }

    // Perform bilinear interpolation
    float height = (1 - u) * (1 - v) * heightMap[xFloor * 256 + yFloor] +
        u * (1 - v) * heightMap[(xFloor + 1 ) * 255 + yFloor] +
        (1 - u) * v * heightMap[xFloor *255 + yFloor + 1] +
        u * v * heightMap[(xFloor + 1) * 255 + yFloor + 1];

    return height;
}

// Generowanie siatki terenu na podstawie mapy wysokoœci
void TerrainMesh::GenerateTerrain()
{
    float x, y, z;
    vertices.clear();
    indices.clear();
    verticesNormals.clear();

    int imgDataIndex = 0;
    // Generowanie wierzcho³ków
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            x = j;
            z = i;
            if (!regenerateDataFromImage) {
                y = Get(j, i);
                heightMapData[imgDataIndex] = static_cast<unsigned char>(y);
            }
            else {
                y = InterpolateHeight(heightMapData, i, j);

            }
            vertices.push_back(z);
            vertices.push_back(y);
            vertices.push_back(x);
            imgDataIndex++;
        }
    }

    // generowanie indeksów
    for (int i = 0; i < HEIGHT - 1; i++) {
        unsigned int upIdStart = i * HEIGHT;
        unsigned int downIdStart = (i + 1) * HEIGHT;
        for (int j = 0; j < WIDTH - 1; j++) {
            indices.push_back(upIdStart + j);
            indices.push_back(downIdStart + j);
            indices.push_back(upIdStart + j + 1);

            indices.push_back(upIdStart + j + 1);
            indices.push_back(downIdStart + j);
            indices.push_back(downIdStart + j + 1);
        }
    }

    GenerateNormals();

    // ³¹czenie tablic wierzcho³ków i normalnych w jedn¹
    for (unsigned int i = 0; i < vertices.size(); i += 3) {
        verticesNormals.push_back(vertices[i]);
        verticesNormals.push_back(vertices[i + 1]);
        verticesNormals.push_back(vertices[i + 2]);

        verticesNormals.push_back(normalsG[i]);
        verticesNormals.push_back(normalsG[i + 1]);
        verticesNormals.push_back(normalsG[i + 2]);
    }

    if (regenerateDataFromImage) {
        regenerateDataFromImage = false;
    }
}

// Funkcja generuj¹ca wektory normalne dla siatki wierzcho³ków
void TerrainMesh::GenerateNormals() {
    normalsG.clear();
    normalsG.resize(vertices.size());

    for (int i = 0; i < indices.size(); i += 3) {
        glm::vec3 v1 = glm::vec3(vertices[indices[i]*3], vertices[indices[i] * 3 +1], vertices[indices[i] * 3 +2]);
        glm::vec3 v2 = glm::vec3(vertices[indices[i+1] * 3], vertices[indices[i+1] * 3 +1], vertices[indices[i+1] * 3 +2]);
        glm::vec3 v3 = glm::vec3(vertices[indices[i+2] * 3], vertices[indices[i+2] * 3 +1], vertices[indices[i+2] * 3 +2]);

        glm::vec3 edge1 = v2 - v1;
        glm::vec3 edge2 = v3 - v1;

        glm::vec3 normal = glm::normalize(glm::cross(edge2, edge1));

        normalsG[indices[i] * 3] = normal.x;
        normalsG[indices[i] * 3 + 1] = (normal.y);
        normalsG[indices[i] * 3 + 2] = normal.z;

        normalsG[indices[i+1] * 3] = normal.x;
        normalsG[indices[i+1] * 3 + 1] = (normal.y);
        normalsG[indices[i+1] * 3 + 2] = normal.z;
         
        normalsG[indices[i+2] * 3] = normal.x;
        normalsG[indices[i+2] * 3 + 1] = (normal.y);
        normalsG[indices[i+2] * 3 + 2] = normal.z;
    }
}