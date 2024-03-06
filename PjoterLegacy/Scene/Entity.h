#pragma once

#include <string>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../Renderer/Texture.h"
#include "Mesh.h"

class SceneManager;

/// <summary>
/// Okreœla obiekt sceny.
/// </summary>
class Entity {
private:
	static size_t ID;
	size_t objectID;
protected:
	std::string name;
	SceneManager* parentSceneManager = NULL;

	//MeshRenderer meshRenderer;
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	Mesh* mesh = nullptr;

	//Material material;
	Texture* texture = nullptr;

	Shader* shader;
public:
	/// <summary>
	/// Konstruktor g³ówny obiektu sceny.
	/// </summary>
	Entity();
	/// <summary>
	/// Konstruktor kopiuj¹cy obiektu sceny.
	/// </summary>
	Entity(const Entity& other) = delete;
	void operator=(const Entity&) = delete;
	~Entity();

	/// <summary>
	/// Zwraca unikalny identyfikator obiektu sceny.
	/// </summary>
	/// <returns>unikalny identyfikator obiektu sceny</returns>
	size_t GetID() const;

	/// <summary>
	/// Ustawia wskaŸnik odwo³uj¹cy siê do sceny, na której jest obiekt.
	/// </summary>
	/// <param name="parentSceneManager">wskaŸnik na obiekt mened¿era scen</param>
	void SetParentSceneManager(SceneManager* parentSceneManager);
	/// <summary>
	/// Zwraca wspaŸnik do obiektu mened¿era scen.
	/// </summary>
	/// <returns>wspaŸnik do obiektu mened¿era scen</returns>
	SceneManager* GetSceneManager() const;

	/// <summary>
	/// Metoda wykonywana przy dodawaniu obiektu do sceny.
	/// </summary>
	virtual void Start() {};
	/// <summary>
	/// Metoda wykonywana w ka¿dej pêtli aplikacji.
	/// </summary>
	virtual void Update() {};
	/// <summary>
	/// Metoda wykonywana podczas niszczenia (usuwania ze sceny) obiektu.
	/// Usuwa równie¿ z pamiêci obiekty komponentów.
	/// </summary>
	virtual void Destroy() {};
	virtual void RefreshMesh() {};

	/// <summary>
	/// Metoda statyczna odpowiadaj¹ca za obs³ugê klawiatury (podstawowego zestawu znaków).
	/// </summary>
	/// <param name="key">kod wciœniêtego klawisza</param>
	/// <param name="x">wspó³rzêdna Y myszy</param>
	/// <param name="y">wspó³rzêdna Y myszy</param>
	virtual void OnKeyboard(unsigned char key, int x, int y) {}
	/// <summary>
	/// Metoda statyczna odpowiadaj¹ca za obs³ugê klawiatury (znaki specjalne).
	/// </summary>
	/// <param name="key">kod wciœniêtego klawisza</param>
	/// <param name="x">wspó³rzêdna Y myszy</param>
	/// <param name="y">wspó³rzêdna Y myszy</param>
	virtual void OnSpecialKeyboard(int key, int x, int y) {}

	/// <summary>
	/// Zwraca skalê obiektu w postaci glm::vec3.
	/// </summary>
	/// <returns>skala obiektu w postaci glm::vec3</returns>
	glm::vec3 GetScale();
	/// <summary>
	/// Zwraca stopieñ obrotu obiektu w postaci glm::quat.
	/// </summary>
	/// <returns></returns>
	glm::quat GetRotation();
	/// <summary>
	/// Zwraca wartoœci przesuniêcia obiektu na scenie jako glm::vec3.
	/// </summary>
	/// <returns>przesuniêcie obiektu na scenie jako glm::vec3</returns>
	glm::vec3 GetTranslation();

	/// <summary>
	/// Przesuwa obiekt o okreœlony wektor glm::vec3.
	/// </summary>
	/// <param name="translateVec">wektor przesuniêcia</param>
	void Translate(glm::vec3 translateVec);
	/// <summary>
	/// Obraca obiekt wzd³ó¿ wybranej osi glm::vec3 oraz o podany k¹t.
	/// </summary>
	/// <param name="rotateVec">wektor osi obrotu</param>
	/// <param name="angle">k¹t obrotu</param>
	void Rotate(glm::vec3 rotateVec, float angle);
	/// <summary>
	/// Skaluje obiekt o podany wektor glm::vec3.
	/// </summary>
	/// <param name="scaleVec">wektor skalowania</param>
	void Scale(glm::vec3 scaleVec);

	/// <summary>
	/// Zwraca macierz transformacji obiektu. Potrzebne przy renderingu.
	/// </summary>
	/// <returns>macierz transformacji obiektu</returns>
	glm::mat4 GetModelMatrix();

	/// <summary>
	/// Zwraca wskaŸnik na obiekt siatki obiektu sceny.
	/// </summary>
	/// <returns>wskaŸnik na obiekt siatki obiektu</returns>
	Mesh* GetMesh() const;

	/// <summary>
	/// Przypisuje do obiektu sceny siatkê.
	/// </summary>
	/// <param name="mesh">wskaŸnik na obiekt siatki</param>
	void SetMesh(Mesh* mesh);

	/// <summary>
	/// Ustawia shader dla tego obiektu sceny.
	/// </summary>
	/// <param name="shader">wskaŸnik na obiekt shadera</param>
	void SetShader(Shader* shader);

	/// <summary>
	/// Zwraca wskaŸnik na obiekt shadera obiektu sceny.
	/// </summary>
	/// <returns>wskaŸnik na obiekt shadera obiektu sceny</returns>
	Shader* GetShader();
};