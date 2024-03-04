#pragma once

#include "../Scene/Mesh.h"
#include "../Renderer/SimplexNoise.h"

#include <vector>
#include "../Scene/TerrainEntity.h"
#include "../Scene/TerrainParameters.h"

/// <summary>
/// Klasa reprezentuj¹ca siatkê terenu.
/// Zawiera metody odpowiedzialne za wygenerowanie siatki na podstawie szumu 
/// i wprowadzonych parametrów.
/// </summary>
class TerrainMesh : public Mesh {
private:

	const int WIDTH = 256; // Szerokoœæ terenu
	const int HEIGHT = 256; // Wysokoœæ terenu

	unsigned char* heightMapData = NULL; // WskaŸnik na dane mapy wysokoœci terenu

	SimplexNoise* noiseGen; // WskaŸnik na obiekt klasy SimplexNoise do generowania szumów

	TerrainParameters* mParams; // WskaŸnik na obiekt klasy TerrainParameters przechowuj¹cy parametry terenu

	std::vector<float> vertices; // Wektor przechowuj¹cy wierzcho³ki terenu
	std::vector<unsigned int> indices; // Wektor przechowuj¹cy indeksy terenu

	std::vector<float> verticesNormals; // Wektor przechowuj¹cy normalne wierzcho³ków terenu oraz wierzcho³ki. Kolejnoœæ (wierzcho³ek, normalna).
	std::vector<float> normalsG; // Wektor przechowuj¹cy tymczasowe wartoœci normalnych, które bêd¹ do³¹czone to wektora: verticesNormals

	bool regenerateDataFromImage = false;

	/// <summary>
	/// Metoda generuj¹ca teren.
	/// </summary>
	void GenerateTerrain();
	/// <summary>
	/// Metoda generuj¹ca normalne.
	/// </summary>
	void GenerateNormals();
	/// <summary>
	/// Metoda zwracaj¹ca wysokoœæ wierzcho³ka Y dla podanych jego wspó³rzêdnych X oraz Z.
	/// </summary>
	float Get(int x, int z);
public:

	/// <summary>
	/// Konstruktor klasy.
	/// </summary>
	/// <param name="params">wskaŸnik na strukturê parametrów terenu</param>
	TerrainMesh(TerrainParameters* params);
	~TerrainMesh();

	/// <summary>
	/// Ustawia now¹ strukturê parametrów terenu.
	/// </summary>
	/// <param name="params">wskaŸnik na strukturê parametrów terenu</param>
	void UpdateTerrainParameters(TerrainParameters* params);

	/// <summary>
	/// Generuje siatkê terenu.
	/// </summary>
	void GenerateTerrainMesh();

	/// <summary>
	/// Wywo³uje akcjê zapisu mapy wysokoœci terenu do pliku graficznego png.
	/// </summary>
	/// <param name="filename">wskaŸnik na nazwê pliku</param>
	void SaveTerrainDataToImage(const char* filename);

	/// <summary>
	/// Zwraca wskaŸnik na tablicê z wartoœciami perm (ziarno generatora terenu).
	/// </summary>
	/// <returns>wskaŸnik na tablicê z wartoœciami perm (ziarno generatora terenu)</returns>
	unsigned char* GetNoiseSeedArray();

	/// <summary>
	/// Uruchamia akcjê zapisu danych terenu do obrazu png.
	/// </summary>
	void SetGenerateDataFromImage();

	/// <summary>
	/// Zwraca wskaŸnik na dane mapy wysokoœci terenu.
	/// </summary>
	/// <returns>wskaŸnik na dane mapy wysokoœci terenu</returns>
	unsigned char* GetHeightMapDataArray();
};
