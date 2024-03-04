#pragma once

#include <string>

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../Renderer/Texture.h"
#include "Mesh.h"

class SceneManager;

/// <summary>
/// Okre�la obiekt sceny.
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
	/// Konstruktor g��wny obiektu sceny.
	/// </summary>
	Entity();
	/// <summary>
	/// Konstruktor kopiuj�cy obiektu sceny.
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
	/// Ustawia wska�nik odwo�uj�cy si� do sceny, na kt�rej jest obiekt.
	/// </summary>
	/// <param name="parentSceneManager">wska�nik na obiekt mened�era scen</param>
	void SetParentSceneManager(SceneManager* parentSceneManager);
	/// <summary>
	/// Zwraca wspa�nik do obiektu mened�era scen.
	/// </summary>
	/// <returns>wspa�nik do obiektu mened�era scen</returns>
	SceneManager* GetSceneManager() const;

	/// <summary>
	/// Metoda wykonywana przy dodawaniu obiektu do sceny.
	/// </summary>
	virtual void Start() {};
	/// <summary>
	/// Metoda wykonywana w ka�dej p�tli aplikacji.
	/// </summary>
	virtual void Update() {};
	/// <summary>
	/// Metoda wykonywana podczas niszczenia (usuwania ze sceny) obiektu.
	/// Usuwa r�wnie� z pami�ci obiekty komponent�w.
	/// </summary>
	virtual void Destroy() {};
	virtual void RefreshMesh() {};

	/// <summary>
	/// Metoda statyczna odpowiadaj�ca za obs�ug� klawiatury (podstawowego zestawu znak�w).
	/// </summary>
	/// <param name="key">kod wci�ni�tego klawisza</param>
	/// <param name="x">wsp�rz�dna Y myszy</param>
	/// <param name="y">wsp�rz�dna Y myszy</param>
	virtual void OnKeyboard(unsigned char key, int x, int y) {}
	/// <summary>
	/// Metoda statyczna odpowiadaj�ca za obs�ug� klawiatury (znaki specjalne).
	/// </summary>
	/// <param name="key">kod wci�ni�tego klawisza</param>
	/// <param name="x">wsp�rz�dna Y myszy</param>
	/// <param name="y">wsp�rz�dna Y myszy</param>
	virtual void OnSpecialKeyboard(int key, int x, int y) {}

	/// <summary>
	/// Zwraca skal� obiektu w postaci glm::vec3.
	/// </summary>
	/// <returns>skala obiektu w postaci glm::vec3</returns>
	glm::vec3 GetScale();
	/// <summary>
	/// Zwraca stopie� obrotu obiektu w postaci glm::quat.
	/// </summary>
	/// <returns></returns>
	glm::quat GetRotation();
	/// <summary>
	/// Zwraca warto�ci przesuni�cia obiektu na scenie jako glm::vec3.
	/// </summary>
	/// <returns>przesuni�cie obiektu na scenie jako glm::vec3</returns>
	glm::vec3 GetTranslation();

	/// <summary>
	/// Przesuwa obiekt o okre�lony wektor glm::vec3.
	/// </summary>
	/// <param name="translateVec">wektor przesuni�cia</param>
	void Translate(glm::vec3 translateVec);
	/// <summary>
	/// Obraca obiekt wzd�� wybranej osi glm::vec3 oraz o podany k�t.
	/// </summary>
	/// <param name="rotateVec">wektor osi obrotu</param>
	/// <param name="angle">k�t obrotu</param>
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
	/// Zwraca wska�nik na obiekt siatki obiektu sceny.
	/// </summary>
	/// <returns>wska�nik na obiekt siatki obiektu</returns>
	Mesh* GetMesh() const;

	/// <summary>
	/// Przypisuje do obiektu sceny siatk�.
	/// </summary>
	/// <param name="mesh">wska�nik na obiekt siatki</param>
	void SetMesh(Mesh* mesh);

	/// <summary>
	/// Ustawia shader dla tego obiektu sceny.
	/// </summary>
	/// <param name="shader">wska�nik na obiekt shadera</param>
	void SetShader(Shader* shader);

	/// <summary>
	/// Zwraca wska�nik na obiekt shadera obiektu sceny.
	/// </summary>
	/// <returns>wska�nik na obiekt shadera obiektu sceny</returns>
	Shader* GetShader();
};