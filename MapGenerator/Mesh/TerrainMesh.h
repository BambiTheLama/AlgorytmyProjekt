#pragma once

#include "../Scene/Mesh.h"
#include "../Renderer/SimplexNoise.h"

#include <vector>
#include "../Scene/TerrainEntity.h"
#include "../Scene/TerrainParameters.h"

/// <summary>
/// Klasa reprezentuj�ca siatk� terenu.
/// Zawiera metody odpowiedzialne za wygenerowanie siatki na podstawie szumu 
/// i wprowadzonych parametr�w.
/// </summary>
class TerrainMesh : public Mesh {
private:

	const int WIDTH = 256; // Szeroko�� terenu
	const int HEIGHT = 256; // Wysoko�� terenu

	unsigned char* heightMapData = NULL; // Wska�nik na dane mapy wysoko�ci terenu

	SimplexNoise* noiseGen; // Wska�nik na obiekt klasy SimplexNoise do generowania szum�w

	TerrainParameters* mParams; // Wska�nik na obiekt klasy TerrainParameters przechowuj�cy parametry terenu

	std::vector<float> vertices; // Wektor przechowuj�cy wierzcho�ki terenu
	std::vector<unsigned int> indices; // Wektor przechowuj�cy indeksy terenu

	std::vector<float> verticesNormals; // Wektor przechowuj�cy normalne wierzcho�k�w terenu oraz wierzcho�ki. Kolejno�� (wierzcho�ek, normalna).
	std::vector<float> normalsG; // Wektor przechowuj�cy tymczasowe warto�ci normalnych, kt�re b�d� do��czone to wektora: verticesNormals

	bool regenerateDataFromImage = false;

	/// <summary>
	/// Metoda generuj�ca teren.
	/// </summary>
	void GenerateTerrain();
	/// <summary>
	/// Metoda generuj�ca normalne.
	/// </summary>
	void GenerateNormals();
	/// <summary>
	/// Metoda zwracaj�ca wysoko�� wierzcho�ka Y dla podanych jego wsp�rz�dnych X oraz Z.
	/// </summary>
	float Get(int x, int z);
public:

	/// <summary>
	/// Konstruktor klasy.
	/// </summary>
	/// <param name="params">wska�nik na struktur� parametr�w terenu</param>
	TerrainMesh(TerrainParameters* params);
	~TerrainMesh();

	/// <summary>
	/// Ustawia now� struktur� parametr�w terenu.
	/// </summary>
	/// <param name="params">wska�nik na struktur� parametr�w terenu</param>
	void UpdateTerrainParameters(TerrainParameters* params);

	/// <summary>
	/// Generuje siatk� terenu.
	/// </summary>
	void GenerateTerrainMesh();

	/// <summary>
	/// Wywo�uje akcj� zapisu mapy wysoko�ci terenu do pliku graficznego png.
	/// </summary>
	/// <param name="filename">wska�nik na nazw� pliku</param>
	void SaveTerrainDataToImage(const char* filename);

	/// <summary>
	/// Zwraca wska�nik na tablic� z warto�ciami perm (ziarno generatora terenu).
	/// </summary>
	/// <returns>wska�nik na tablic� z warto�ciami perm (ziarno generatora terenu)</returns>
	unsigned char* GetNoiseSeedArray();

	/// <summary>
	/// Uruchamia akcj� zapisu danych terenu do obrazu png.
	/// </summary>
	void SetGenerateDataFromImage();

	/// <summary>
	/// Zwraca wska�nik na dane mapy wysoko�ci terenu.
	/// </summary>
	/// <returns>wska�nik na dane mapy wysoko�ci terenu</returns>
	unsigned char* GetHeightMapDataArray();
};
