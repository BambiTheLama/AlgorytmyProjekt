#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>
#include "../Scene/SceneManager.h"
#include "../Scene/TerrainParameters.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

/// <summary>
/// G��wna klasa silnika aplikacji. Odpowiada za inicjalizacj� i zwalnianie zasob�w.
/// </summary>
class EngineCore {
private:
	GLFWwindow* window;
	SceneManager* sceneManager;
	int width = 960;
	int lastWidth = 960;
	int height = 600;
	int lastHeight = 600;
	std::string windowTitle;
	bool fullscreen = false;
	bool disabledCursor = true;

	int screenResolutionHeight = 1080;

	static void OnResize(int w, int h);

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	glm::mat4 view;
	glm::mat4 projection;

	bool orthoCamera = false;

	void UpdateProjectionCamera();

	glm::vec3 cameraPos = glm::vec3(-10.0f, 20.0f, -7.0f);
	glm::vec3 cameraFront = glm::vec3(0.578f, -0.578f, 0.578f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float lastX;
	float lastY;
	bool firstMouse = true;
	float yaw = 46.70f;
	float pitch = -37.10f;

	float zNear = 1.0f;
	float zFar = 20.0f;
	float fov = 50.0f;
	// Ortho camera
	float zNearOrtho = 0.1f;
	float zFarOrtho = 100.0f;

	Renderer renderer;

	TerrainParameters mTerrainParameters;

protected:
	/// <summary>
	/// Chroniony konstruktor klasy, zastosowanie wzorca Singleton.
	/// </summary>
	EngineCore();
	~EngineCore();

	static EngineCore* engineCore;

public:
	/// <summary>
	/// Wy��czenie konstruktora kopiuj�cego, zastosowanie wzorca Singleton.
	/// </summary>
	/// <param name="other"></param>
	EngineCore(EngineCore& other) = delete;
	/// <summary>
	/// Wy��czenie przeci��enia operatora kopiowania, zastosowanie wzorca Singleton.
	/// </summary>
	/// <param name=""></param>
	void operator=(const EngineCore&) = delete;

	/// <summary>
	/// Zwr�cenie wska�nika na instancj� klasy EngineCore.
	/// Je�eli obiekt nie istnia� jest tworzony.
	/// </summary>
	/// <returns>wska�nik na instancj� klasy EngineCore</returns>
	static EngineCore* GetInstance();

	/// <summary>
	/// Umo�liwia zrealizowanie pomiaru czasu od wyrenderowania poprzedniej klatki do wyrenderowania 
	/// nowej klatki animacji (delta time).
	/// </summary>
	void OnUpdate();

	/// <summary>
	/// Zwraca czas od wyrenderowania poprzedniej klatki do wyrenderowania 
	/// nowej klatki animacji (delta time).
	/// </summary>
	/// <returns>delta time</returns>
	float GetDeltaTime();

	/// <summary>
	/// Zwraca wska�nik na mened�er scen.
	/// </summary>
	/// <returns>wska�nik na mened�er scen</returns>
	SceneManager* GetSceneManager();

	/// <summary>
	/// Inicjalizuje okno gry oraz podstawowe parametry silnika aplikacji.
	/// </summary>
	/// <param name="argc">wska�nik na ilo�c element�w tablicy argument�w</param>
	/// <param name="argv">wska�nik na tablic� argument�w startowych programu</param>
	/// <param name="title">tutu� okna</param>
	/// <param name="width">szeroko�� okna</param>
	/// <param name="height">wysoko�� okna</param>
	void InitWindow(const std::string& title, int width, int height);
	/// <summary>
	/// Ko�czy dzia�anie silnika aplikacji i zwalnia zaj�te zasoby.
	/// </summary>
	void Deinit();

	/// <summary>
	/// Zwraca szeroko�� okna.
	/// </summary>
	/// <returns>szeroko�� okna</returns>
	int GetWidth() const;
	/// <summary>
	/// Zwraca wysoko�� okna.
	/// </summary>
	/// <returns>wysoko�� okna</returns>
	int GetHeight() const;

	/// <summary>
	/// Zmienia tryb kamery na:
	/// orthoCamera jest prawd� - to kamera przechodzi w tryb prostok�tny
	/// orthoCamera jest fa�sz - to kamera przechodzi w tryb perspektywiczny
	/// </summary>
	void SetOrthoCamera(bool orthoCamera);
	/// <summary>
	/// Zwraca aktualny tryb kamery prostok�tny lub perspektywiczny w zale�no�ci jaki jest ustawiony.
	/// </summary>
	bool IsOrthoCamera();

	/// <summary>
	/// Ustawia parametry kamery perspektywicznej.
	/// </summary>
	/// <param name="fov">pole widzenia</param>
	/// <param name="zNear">pocz�tek odleg�o�ci pola widzenia</param>
	/// <param name="zFar">koniec odleg�o�ci pola widzenia (maksymalny zasi�g widzenia)</param>
	void SetPersCameraParams(float fov, float zNear, float zFar);
	/// <summary>
	/// Zwraca aktualne parametry kamery perspektywicznej.
	/// </summary>
	/// <param name="fov">wska�nik na zmienn�, kt�ra przechowa parametr pola widzenia</param>
	/// <param name="zNear">wska�nik na zmienn�, kt�ra przechowa parametr: pocz�tkek odleg�o�ci pola widzenia</param>
	/// <param name="zFar">wska�nik na zmienn�, kt�ra przechowa parametr: koniec odleg�o�ci pola widzenia (maksymalny zasi�g widzenia)</param>
	void GetPersCameraParams(float* fov, float* zNear, float* zFar);

	/// <summary>
	/// Ustawia parametry kamery prostok�tnej.
	/// </summary>
	/// <param name="zNear">pocz�tek odleg�o�ci pola widzenia</param>
	/// <param name="zFar">koniec odleg�o�ci pola widzenia (maksymalny zasi�g widzenia)</param>
	void SetOrthoCameraParams(float zNear, float zFar);
	/// <summary>
	/// Zwraca aktualne parametry kamery prostok�tnej.
	/// </summary>
	/// <param name="zNear">wska�nik na zmienn�, kt�ra przechowa parametr: pocz�tkek odleg�o�ci pola widzenia</param>
	/// <param name="zFar">wska�nik na zmienn�, kt�ra przechowa parametr: koniec odleg�o�ci pola widzenia (maksymalny zasi�g widzenia)</param>
	void GetOrthoCameraParams(float* zNear, float* zFar);

	/// <summary>
	/// Metoda wywo�ywana w momencie akcji ruszenia wska�nikiem myszy.
	/// </summary>
	/// <param name="window">wska�nik na okno aplikacji</param>
	/// <param name="xpos">wsp�rz�dna X kursora myszy</param>
	/// <param name="ypos">wsp�rz�dna Y kursora myszy</param>
	static void OnMouse(GLFWwindow* window, double xpos, double ypos);
	/// <summary>
	/// Metoda wywo�ywana w momencie akcji zmiany stanu klawisza na klawiaturze.
	/// </summary>
	/// <param name="window">wska�nik na okno aplikacji</param>
	void KeyInput(GLFWwindow* window);

	/// <summary>
	/// Zwraca wska�nik na obiekt renderowania.
	/// </summary>
	/// <returns>wska�nik na obiekt renderowania</returns>
	Renderer* GetRenderer();

	/// <summary>
	/// Zwraca glm::mat4, macierz projekcji.
	/// </summary>
	/// <returns>glm::mat4, macierz projekcji</returns>
	glm::mat4 GetProjection() const;
	/// <summary>
	/// Zwraca glm::mat4, macierz widoku.
	/// </summary>
	/// <returns>glm::mat4, macierz widoku</returns>
	glm::mat4 GetView() const;
	/// <summary>
	/// Zwraca glm::vec3 z pozycj� kamery.
	/// </summary>
	/// <returns>glm::vec3 z pozycj� kamery</returns>
	glm::vec3 GetCameraPos() const;

	/// <summary>
	/// Zwraca wska�nik na struktur� parametr�w terenu.
	/// </summary>
	/// <returns>wska�nik na struktur� parametr�w terenu</returns>
	TerrainParameters* GetTerrainParameters();
};