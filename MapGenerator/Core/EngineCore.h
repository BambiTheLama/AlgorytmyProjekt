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
/// G³ówna klasa silnika aplikacji. Odpowiada za inicjalizacjê i zwalnianie zasobów.
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
	/// Wy³¹czenie konstruktora kopiuj¹cego, zastosowanie wzorca Singleton.
	/// </summary>
	/// <param name="other"></param>
	EngineCore(EngineCore& other) = delete;
	/// <summary>
	/// Wy³¹czenie przeci¹¿enia operatora kopiowania, zastosowanie wzorca Singleton.
	/// </summary>
	/// <param name=""></param>
	void operator=(const EngineCore&) = delete;

	/// <summary>
	/// Zwrócenie wskaŸnika na instancjê klasy EngineCore.
	/// Je¿eli obiekt nie istnia³ jest tworzony.
	/// </summary>
	/// <returns>wskaŸnik na instancjê klasy EngineCore</returns>
	static EngineCore* GetInstance();

	/// <summary>
	/// Umo¿liwia zrealizowanie pomiaru czasu od wyrenderowania poprzedniej klatki do wyrenderowania 
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
	/// Zwraca wskaŸnik na menedŸer scen.
	/// </summary>
	/// <returns>wskaŸnik na menedŸer scen</returns>
	SceneManager* GetSceneManager();

	/// <summary>
	/// Inicjalizuje okno gry oraz podstawowe parametry silnika aplikacji.
	/// </summary>
	/// <param name="argc">wskaŸnik na iloœc elementów tablicy argumentów</param>
	/// <param name="argv">wskaŸnik na tablicê argumentów startowych programu</param>
	/// <param name="title">tutu³ okna</param>
	/// <param name="width">szerokoœæ okna</param>
	/// <param name="height">wysokoœæ okna</param>
	void InitWindow(const std::string& title, int width, int height);
	/// <summary>
	/// Koñczy dzia³anie silnika aplikacji i zwalnia zajête zasoby.
	/// </summary>
	void Deinit();

	/// <summary>
	/// Zwraca szerokoœæ okna.
	/// </summary>
	/// <returns>szerokoœæ okna</returns>
	int GetWidth() const;
	/// <summary>
	/// Zwraca wysokoœæ okna.
	/// </summary>
	/// <returns>wysokoœæ okna</returns>
	int GetHeight() const;

	/// <summary>
	/// Zmienia tryb kamery na:
	/// orthoCamera jest prawd¹ - to kamera przechodzi w tryb prostok¹tny
	/// orthoCamera jest fa³sz - to kamera przechodzi w tryb perspektywiczny
	/// </summary>
	void SetOrthoCamera(bool orthoCamera);
	/// <summary>
	/// Zwraca aktualny tryb kamery prostok¹tny lub perspektywiczny w zale¿noœci jaki jest ustawiony.
	/// </summary>
	bool IsOrthoCamera();

	/// <summary>
	/// Ustawia parametry kamery perspektywicznej.
	/// </summary>
	/// <param name="fov">pole widzenia</param>
	/// <param name="zNear">pocz¹tek odleg³oœci pola widzenia</param>
	/// <param name="zFar">koniec odleg³oœci pola widzenia (maksymalny zasiêg widzenia)</param>
	void SetPersCameraParams(float fov, float zNear, float zFar);
	/// <summary>
	/// Zwraca aktualne parametry kamery perspektywicznej.
	/// </summary>
	/// <param name="fov">wskaŸnik na zmienn¹, która przechowa parametr pola widzenia</param>
	/// <param name="zNear">wskaŸnik na zmienn¹, która przechowa parametr: pocz¹tkek odleg³oœci pola widzenia</param>
	/// <param name="zFar">wskaŸnik na zmienn¹, która przechowa parametr: koniec odleg³oœci pola widzenia (maksymalny zasiêg widzenia)</param>
	void GetPersCameraParams(float* fov, float* zNear, float* zFar);

	/// <summary>
	/// Ustawia parametry kamery prostok¹tnej.
	/// </summary>
	/// <param name="zNear">pocz¹tek odleg³oœci pola widzenia</param>
	/// <param name="zFar">koniec odleg³oœci pola widzenia (maksymalny zasiêg widzenia)</param>
	void SetOrthoCameraParams(float zNear, float zFar);
	/// <summary>
	/// Zwraca aktualne parametry kamery prostok¹tnej.
	/// </summary>
	/// <param name="zNear">wskaŸnik na zmienn¹, która przechowa parametr: pocz¹tkek odleg³oœci pola widzenia</param>
	/// <param name="zFar">wskaŸnik na zmienn¹, która przechowa parametr: koniec odleg³oœci pola widzenia (maksymalny zasiêg widzenia)</param>
	void GetOrthoCameraParams(float* zNear, float* zFar);

	/// <summary>
	/// Metoda wywo³ywana w momencie akcji ruszenia wskaŸnikiem myszy.
	/// </summary>
	/// <param name="window">wskaŸnik na okno aplikacji</param>
	/// <param name="xpos">wspó³rzêdna X kursora myszy</param>
	/// <param name="ypos">wspó³rzêdna Y kursora myszy</param>
	static void OnMouse(GLFWwindow* window, double xpos, double ypos);
	/// <summary>
	/// Metoda wywo³ywana w momencie akcji zmiany stanu klawisza na klawiaturze.
	/// </summary>
	/// <param name="window">wskaŸnik na okno aplikacji</param>
	void KeyInput(GLFWwindow* window);

	/// <summary>
	/// Zwraca wskaŸnik na obiekt renderowania.
	/// </summary>
	/// <returns>wskaŸnik na obiekt renderowania</returns>
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
	/// Zwraca glm::vec3 z pozycj¹ kamery.
	/// </summary>
	/// <returns>glm::vec3 z pozycj¹ kamery</returns>
	glm::vec3 GetCameraPos() const;

	/// <summary>
	/// Zwraca wskaŸnik na strukturê parametrów terenu.
	/// </summary>
	/// <returns>wskaŸnik na strukturê parametrów terenu</returns>
	TerrainParameters* GetTerrainParameters();
};