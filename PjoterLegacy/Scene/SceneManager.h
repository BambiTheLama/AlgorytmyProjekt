#pragma once

#include <vector>
#include <string>

#include "Entity.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "TerrainParameters.h"

class EngineCore;
class TerrainEntity;

/// <summary>
/// Odpowiada za zarz¹dzanie scen¹ i jej obiektami aplikacji (Entity).
/// </summary>
class SceneManager {
private:
	std::vector<Entity*> entities;
	std::vector<Entity*> entitiesToDelete;

	std::string name;
	EngineCore* core;

	Shader shader;
	Shader shaderNormalDebug;

	TerrainParameters* _terrainParameters;

	TerrainEntity* terrainEntity = NULL;

	// camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float lastX;
	float lastY;
	bool firstMouse;
	float yaw;
	float pitch;

	/// <summary>
	/// Odpowiada za usuniêcie z pamiêci obiektów, które zosta³y usuniête 
	/// ze sceny i nie bêd¹ wiêcej u¿ywane.
	/// Inicjuje równie¿ usuniêcie z pamiêci komponentu ka¿dego usuwanych obiektów.
	/// </summary>
	void DeleteGarbageEntities();

	// Oswietlenie:
	float lightNoneSpec[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightPos[3] = { 0, 1, 2 };
	float spotLightPos[4] = { 0, 40, 1, 1.0f };
	float globAmb[4] = { 0.05, 0.05, 0.05, 1.0 };

	bool showNormals = false;

	/// <summary>
	/// Inicjalizuje parametry sceny aplikacji.
	/// </summary>
	void InitSceneParams();
protected:
	/// <summary>
	/// G³ówny ukryty konstruktor klasy.
	/// </summary>
	/// <param name="core">wskaŸnik na obiekt zarz¹dzaj¹cy silnikiem</param>
	SceneManager(EngineCore* core);

	static SceneManager* sceneManager;

public:
	~SceneManager();
	/// <summary>
	/// Wy³¹czenie konstruktora kopiuj¹cego, zastosowanie wzorca Singleton.
	/// </summary>
	/// <param name="other"></param>
	SceneManager(EngineCore& other) = delete;
	/// <summary>
	/// Wy³¹czenie przeci¹¿enia operatora kopiowania, zastosowanie wzorca Singleton.
	/// </summary>
	/// <param name=""></param>
	void operator=(const SceneManager&) = delete;

	/// <summary>
	/// Zwrócenie wskaŸnika na instancjê klasy EngineCore.
	/// Je¿eli obiekt nie istnia³ jest tworzony.
	/// </summary>
	/// <returns>wskaŸnik na instancjê klasy EngineCore</returns>
	static SceneManager* GetInstance(EngineCore* core);

	/// <summary>
	/// Metoda wykonywana przy uruchamianiu sceny.
	/// Inicjuje podstawowe elementy oraz przechodzi do funkcji z g³ówn¹ pêtl¹.
	/// </summary>
	void Start();
	/// <summary>
	/// Metoda zawieraj¹ca g³ówn¹ pêtlê aplikacji. Odpowiada za rysowanie i odœwie¿anie stanu obiektów sceny.
	/// </summary>
	static void OnDraw();
	void Update();
	/// <summary>
	/// Metoda wykonywana podczas wy³¹czania sceny np. przy zamykaniu okna z aplikacj¹.
	/// </summary>
	void Deinit();

	/// <summary>
	/// Ustawia na scenie obiekt terenu.
	/// </summary>
	/// <param name="terrainEntity">wskaŸnik na obiekt terenu terrainEntity</param>
	void SetTerrainEntity(TerrainEntity* terrainEntity);

	/// <summary>
	/// Dodaje obiekt do sceny.
	/// </summary>
	/// <param name="ent">wskaŸnik na obiekt aplikacji</param>
	void AddEntity(Entity* ent);
	/// <summary>
	/// Zwraca wskaŸnik na obiekt sceny i posiadaj¹cy podany identyfikator.
	/// </summary>
	/// <param name="id">identyfikator liczbowy obiektu sceny</param>
	/// <returns></returns>
	Entity* GetEntityById(int id);
	/// <summary>
	/// Usuwa ze sceny obiekt sceny o podanym identyfikatorze o ile taki istnieje.
	/// PóŸniej obiekt sceny wraz z komponentami zostaje usuniêty z pamiêci.
	/// </summary>
	/// <param name="id"></param>
	void RemoveEntityById(size_t id);
	/// <summary>
	/// £aduje now¹ scenê. Poprzednio za³adowana scena jest czyszczona, a pamiêæ jest zwalniana.
	/// </summary>
	/// <param name="sceneGameObjects">wskaŸnik na vector z obiektami nowej sceny</param>
	/// <param name="sceneName">nazwa nowej sceny</param>
	void LoadNewScene(const std::vector<Entity*>& sceneGameObjects, const std::string& sceneName);

	/// <summary>
	/// Ustawia nazwê sceny.
	/// </summary>
	/// <param name="name">nazwa sceny (obiekt std::string)</param>
	void SetName(const std::string& name);
	/// <summary>
	/// Zwraca nazwê sceny.
	/// </summary>
	/// <returns>nazwa sceny (obiekt std::string)</returns>
	std::string GetName() const;

	/// <summary>
	/// Ustawia now¹ pozycjê kamery na scenie.
	/// </summary>
	/// <param name="cameraPos">glm::vec3 z pozycj¹ do ustawienia</param>
	void SetCameraPos(glm::vec3 cameraPos);
	/// <summary>
	/// Zwraca pozycjê kamery na scenie jako glm::vec3.
	/// </summary>
	glm::vec3 GetCameraPos();

	/// <summary>
	/// Ustawia now¹ pozycjê dla oœwietlenia g³ównego na scenie.
	/// </summary>
	/// <param name="lightPos">wskaŸnik na tablicê 4 elementow¹, która zawiera wspó³rzêdne po³o¿enia</param>
	void SetLightPos(float* lightPos);
	/// <summary>
	/// Zwraca pozycjê dla oœwietlenia g³ównego na scenie.
	/// Zwraca wskaŸnik na tablicê 4 elementow¹, która zawiera wspó³rzêdne po³o¿enia.
	/// </summary>
	float* GetLightPos();
};