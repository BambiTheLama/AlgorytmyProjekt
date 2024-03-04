#include "SceneManager.h"
#include "../Core/EngineCore.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/VertexBufferLayout.h"
#include "../Core/DataParametersSaveManager.h"
#include "TerrainEntity.h"
#include "../Core/ImageManager.h"

SceneManager* SceneManager::sceneManager = nullptr;
std::vector<Entity*>::size_type entityCount;

SceneManager::SceneManager(EngineCore* core)
{
	this->core = core;
	lastX = core->GetWidth() / 2;
	lastY = core->GetHeight() / 2;
	firstMouse = true;
	yaw = -90.0f;
	pitch = 0.0f;

	_terrainParameters = core->GetTerrainParameters();
}

SceneManager::~SceneManager()
{
	this->sceneManager = nullptr;
	delete sceneManager;
}

SceneManager* SceneManager::GetInstance(EngineCore* core)
{
	if (sceneManager == nullptr) {
		sceneManager = new SceneManager(core);
	}
	return sceneManager;
}

void SceneManager::Start()
{
	InitSceneParams();
}

void SceneManager::OnDraw()
{
	if (!sceneManager)
		return;

	sceneManager->Update();
}

bool isTerrainParamsUpdated = false;
int errorDispFrames = 0;
void SceneManager::Update()
{
	ImGui::Checkbox("Pokaz wektory normalne", &sceneManager->showNormals);
	if (ImGui::SliderFloat("Skala", &(_terrainParameters->scale), 100, 1000) ||
		ImGui::SliderInt("Wysokosc", &(_terrainParameters->height), 5, 255) ||
		ImGui::SliderInt("Oktawy", &(_terrainParameters->octaves), 2, 32) ||
		ImGui::SliderFloat("Trwalosc", &(_terrainParameters->persistence), 0.5, 10) ||
		ImGui::SliderFloat("Zwielokrotnienie", &(_terrainParameters->lacunarity), 0, 4) ||
		ImGui::SliderFloat("Potegowanie", &(_terrainParameters->exponentiation), 0, 10) ||
		ImGui::Button("Odswiez teren")) {
		isTerrainParamsUpdated = true;
	}

	if (ImGui::Button("Wylosuj nowe ziarno")) {
		_terrainParameters->regenerateTerrain = true;
		isTerrainParamsUpdated = true;
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::Separator();
	ImGui::Text("Zapis/ odczyt:");
	if (ImGui::Button("Zapisz szum do pliku png")) {
		terrainEntity->SaveHeightMapToImage("mapaWysokosci.png");
	}

	if (ImGui::Button("Wczytaj dane z obrazu png")) {
		bool opStatus = ImageManager::LoadDataFromImageToArray("mapaWysokosci.png", terrainEntity->GetHeightMapDataArray());

		if (!opStatus)
			errorDispFrames = 280;
		else {
			terrainEntity->SetGenerateDataFromImage();
			isTerrainParamsUpdated = true;
		}
	}

	if (ImGui::Button("Zapisz dane szumu oraz ziarno")) {
		DataParametersSaveManager::SaveParametersToFile("dane.txt",
			_terrainParameters, terrainEntity->GetSeedArray());
	}

	if (ImGui::Button("Wczytaj dane szumu oraz ziarno")) {
		bool opStatus = DataParametersSaveManager::LoadParametersFromFile("dane.txt",
			_terrainParameters, terrainEntity->GetSeedArray());

		if (!opStatus)
			errorDispFrames = 280;
		else
			isTerrainParamsUpdated = true;
	}

	if (errorDispFrames) {
		ImGui::Text("Nastapil blad oczytu pliku.");
		errorDispFrames--;
	}

	for (auto& GO : entities) {
		Mesh* goMesh = GO->GetMesh();

		if (goMesh) {

			glm::vec3 cameraPos = core->GetCameraPos();

			GO->GetShader()->Bind();

			//GO->GetShader()->SetUniform3f("u_Color", 0.0f, 1.0f, 0.0f);
			//GO->GetShader()->SetUniform3f("u_Color", 0.0f, 0.5f, 0.31f);
			GO->GetShader()->SetUniform3f("u_LightColor", 1.0f, 1.0f, 1.0f);
			GO->GetShader()->SetUniform3f("lightPos", lightPos[0], lightPos[1], lightPos[2]);
			//GO->GetShader()->SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			GO->GetShader()->SetUniformMat4f("u_View", core->GetView());
			GO->GetShader()->SetUniformMat4f("u_Model", GO->GetModelMatrix());
			GO->GetShader()->SetUniformMat4f("u_Projection", core->GetProjection());

			core->GetRenderer()->Draw(goMesh->GetVa(), goMesh->GetIb(), *(GO->GetShader()));
			GO->GetShader()->Unbind();

			if (sceneManager->showNormals) {
				shaderNormalDebug.Bind();
				shaderNormalDebug.SetUniformMat4f("view", core->GetView());
				shaderNormalDebug.SetUniformMat4f("model", GO->GetModelMatrix());
				shaderNormalDebug.SetUniformMat4f("projection", core->GetProjection());
				core->GetRenderer()->Draw(goMesh->GetVa(), goMesh->GetIb(), shaderNormalDebug);
				shaderNormalDebug.Unbind();
			}

			if (isTerrainParamsUpdated)
			{
				GO->RefreshMesh();
				isTerrainParamsUpdated = false;
			}
		}

		GO->Update();
	}
	
	DeleteGarbageEntities();
}

void SceneManager::InitSceneParams()
{
	shader.SetPath("Resources/Shaders/basic.shader");
	shaderNormalDebug.SetPath("Resources/Shaders/normalDebug.shader");
}

void SceneManager::Deinit()
{
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		(*it)->Destroy();
		delete (*it);
	}

	DeleteGarbageEntities();
	entities.clear();
}

void SceneManager::AddEntity(Entity* ent)
{
	ent->SetParentSceneManager(this);
	ent->Start();
	entities.push_back(ent);
}

void SceneManager::SetTerrainEntity(TerrainEntity* terrainEntity)
{
	this->terrainEntity = terrainEntity;
	terrainEntity->SetParentSceneManager(this);
	terrainEntity->Start();
	entities.push_back(terrainEntity);
}

Entity* SceneManager::GetEntityById(int id)
{
	for (auto& component : entities) {
		if (component->GetID() == id) { return (component); }
	}
	return nullptr;
}

void SceneManager::RemoveEntityById(size_t id)
{
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		if ((*it)->GetID() == id) {
			(*it)->Destroy();

			entitiesToDelete.push_back((*it));
			entities.erase(it);
			entityCount = entities.size();
			break;
		}
	}
}

void SceneManager::DeleteGarbageEntities()
{
	for (auto& component : entitiesToDelete) {
		delete (component);
	}
	entitiesToDelete.clear();
}

void SceneManager::LoadNewScene(const std::vector<Entity*>& sceneGameObjects, const std::string& sceneName)
{
	Deinit();
	SetName(sceneName);
	for (auto& object : sceneGameObjects) {
		AddEntity(object);
	}
	Start();
}

void SceneManager::SetName(const std::string& name)
{
	this->name = name;
}

std::string SceneManager::GetName() const
{
	return name;
}

void SceneManager::SetCameraPos(glm::vec3 cameraPos)
{
	this->cameraPos = cameraPos;
}

glm::vec3 SceneManager::GetCameraPos()
{
	return cameraPos;
}

void SceneManager::SetLightPos(float* lightPos)
{
	this->lightPos[0] = lightPos[0];
	this->lightPos[1] = lightPos[1];
	this->lightPos[2] = lightPos[2];
}

float* SceneManager::GetLightPos()
{
	return lightPos;
}
