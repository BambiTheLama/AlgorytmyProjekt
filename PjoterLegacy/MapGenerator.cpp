#include <iostream>
#include "Core/EngineCore.h"
#include "Mesh/TerrainMesh.h"
#include "Mesh/PlaneMesh.h"
#ifdef PjoterMode
int main()
{
    EngineCore* core = EngineCore::GetInstance();
    core->InitWindow("Terrax", 1200, 800);
    
    Shader* waterShader = new Shader();
    waterShader->SetPath("Resources/Shaders/water.shader");
    Entity* waterLevel = new Entity();
    waterLevel->SetMesh(new PlaneMesh());
    waterLevel->Translate(glm::vec3(13.0f, 0.14f, 13.0f));
    waterLevel->Scale(glm::vec3(40.0f));
    waterLevel->SetShader(waterShader);

    Shader* terrainShader = new Shader();
    terrainShader->SetPath("Resources/Shaders/basic.shader");
    TerrainEntity* terrainEnt = new TerrainEntity();

    terrainEnt->Translate(glm::vec3(0.0f, 0.0f, 0.0f));
    terrainEnt->Scale(glm::vec3(0.1f));
    terrainEnt->SetShader(terrainShader);

    core->GetSceneManager()->SetTerrainEntity(terrainEnt);
    core->GetSceneManager()->AddEntity(waterLevel);
    core->OnUpdate();

    // Zwolnienie zasobów shadera oraz silnika aplikacji
    delete waterShader;
    delete terrainShader;
    core->Deinit();
 
    return 0;
}
#endif