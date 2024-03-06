#include "TerrainEntity.h"
#include <iostream>

TerrainEntity::TerrainEntity()
{
	EngineCore* core = EngineCore::GetInstance();
	mesh = new TerrainMesh(core->GetTerrainParameters());
}

TerrainEntity::~TerrainEntity()
{
	delete mesh;
}

void TerrainEntity::Update()
{

}

void TerrainEntity::RefreshMesh()
{
	TerrainMesh* terrainMesh = static_cast<TerrainMesh*>(mesh);
	terrainMesh->GenerateTerrainMesh();
}

void TerrainEntity::SaveHeightMapToImage(const char* filename)
{
	TerrainMesh* terrainMesh = static_cast<TerrainMesh*>(mesh);
	terrainMesh->SaveTerrainDataToImage(filename);
}

unsigned char* TerrainEntity::GetSeedArray()
{
	TerrainMesh* terrainMesh = static_cast<TerrainMesh*>(mesh);
	return terrainMesh->GetNoiseSeedArray();
}

void TerrainEntity::SetGenerateDataFromImage()
{
	TerrainMesh* terrainMesh = static_cast<TerrainMesh*>(mesh);
	terrainMesh->SetGenerateDataFromImage();
}

unsigned char* TerrainEntity::GetHeightMapDataArray()
{
	TerrainMesh* terrainMesh = static_cast<TerrainMesh*>(mesh);
	return terrainMesh->GetHeightMapDataArray();
}
