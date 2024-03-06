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
/// Odpowiada za zarz�dzanie scen� i jej obiektami aplikacji (Entity).
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
	/// Odpowiada za usuni�cie z pami�ci obiekt�w, kt�re zosta�y usuni�te 
	/// ze sceny i nie b�d� wi�cej u�ywane.
	/// Inicjuje r�wnie� usuni�cie z pami�ci komponentu ka�dego usuwanych obiekt�w.
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
	/// G��wny ukryty konstruktor klasy.
	/// </summary>
	/// <param name="core">wska�nik na obiekt zarz�dzaj�cy silnikiem</param>
	SceneManager(EngineCore* core);

	static SceneManager* sceneManager;

public:
	~SceneManager();
	/// <summary>
	/// Wy��czenie konstruktora kopiuj�cego, zastosowanie wzorca Singleton.
	/// </summary>
	/// <param name="other"></param>
	SceneManager(EngineCore& other) = delete;
	/// <summary>
	/// Wy��czenie przeci��enia operatora kopiowania, zastosowanie wzorca Singleton.
	/// </summary>
	/// <param name=""></param>
	void operator=(const SceneManager&) = delete;

	/// <summary>
	/// Zwr�cenie wska�nika na instancj� klasy EngineCore.
	/// Je�eli obiekt nie istnia� jest tworzony.
	/// </summary>
	/// <returns>wska�nik na instancj� klasy EngineCore</returns>
	static SceneManager* GetInstance(EngineCore* core);

	/// <summary>
	/// Metoda wykonywana przy uruchamianiu sceny.
	/// Inicjuje podstawowe elementy oraz przechodzi do funkcji z g��wn� p�tl�.
	/// </summary>
	void Start();
	/// <summary>
	/// Metoda zawieraj�ca g��wn� p�tl� aplikacji. Odpowiada za rysowanie i od�wie�anie stanu obiekt�w sceny.
	/// </summary>
	static void OnDraw();
	void Update();
	/// <summary>
	/// Metoda wykonywana podczas wy��czania sceny np. przy zamykaniu okna z aplikacj�.
	/// </summary>
	void Deinit();

	/// <summary>
	/// Ustawia na scenie obiekt terenu.
	/// </summary>
	/// <param name="terrainEntity">wska�nik na obiekt terenu terrainEntity</param>
	void SetTerrainEntity(TerrainEntity* terrainEntity);

	/// <summary>
	/// Dodaje obiekt do sceny.
	/// </summary>
	/// <param name="ent">wska�nik na obiekt aplikacji</param>
	void AddEntity(Entity* ent);
	/// <summary>
	/// Zwraca wska�nik na obiekt sceny i posiadaj�cy podany identyfikator.
	/// </summary>
	/// <param name="id">identyfikator liczbowy obiektu sceny</param>
	/// <returns></returns>
	Entity* GetEntityById(int id);
	/// <summary>
	/// Usuwa ze sceny obiekt sceny o podanym identyfikatorze o ile taki istnieje.
	/// P�niej obiekt sceny wraz z komponentami zostaje usuni�ty z pami�ci.
	/// </summary>
	/// <param name="id"></param>
	void RemoveEntityById(size_t id);
	/// <summary>
	/// �aduje now� scen�. Poprzednio za�adowana scena jest czyszczona, a pami�� jest zwalniana.
	/// </summary>
	/// <param name="sceneGameObjects">wska�nik na vector z obiektami nowej sceny</param>
	/// <param name="sceneName">nazwa nowej sceny</param>
	void LoadNewScene(const std::vector<Entity*>& sceneGameObjects, const std::string& sceneName);

	/// <summary>
	/// Ustawia nazw� sceny.
	/// </summary>
	/// <param name="name">nazwa sceny (obiekt std::string)</param>
	void SetName(const std::string& name);
	/// <summary>
	/// Zwraca nazw� sceny.
	/// </summary>
	/// <returns>nazwa sceny (obiekt std::string)</returns>
	std::string GetName() const;

	/// <summary>
	/// Ustawia now� pozycj� kamery na scenie.
	/// </summary>
	/// <param name="cameraPos">glm::vec3 z pozycj� do ustawienia</param>
	void SetCameraPos(glm::vec3 cameraPos);
	/// <summary>
	/// Zwraca pozycj� kamery na scenie jako glm::vec3.
	/// </summary>
	glm::vec3 GetCameraPos();

	/// <summary>
	/// Ustawia now� pozycj� dla o�wietlenia g��wnego na scenie.
	/// </summary>
	/// <param name="lightPos">wska�nik na tablic� 4 elementow�, kt�ra zawiera wsp�rz�dne po�o�enia</param>
	void SetLightPos(float* lightPos);
	/// <summary>
	/// Zwraca pozycj� dla o�wietlenia g��wnego na scenie.
	/// Zwraca wska�nik na tablic� 4 elementow�, kt�ra zawiera wsp�rz�dne po�o�enia.
	/// </summary>
	float* GetLightPos();
};