#pragma once

#include "Entity.h"
#include "../Mesh/TerrainMesh.h"
#include "../Core/EngineCore.h"

/// <summary>
/// Klasa obiektu sceny terenu.
/// </summary>
class TerrainEntity : public Entity {
private:
        

public:
	TerrainEntity();
	~TerrainEntity();

	/// <summary>
	/// Metoda wykonywana na pocz¹tku sceny, po za³adowaniu.
	/// </summary>
	void Start() {};
	/// <summary>
	/// Metoda wykonywana w ka¿dej pêtli aplikacji.
	/// </summary>
	void Update();
	/// <summary>
	/// Metoda wykonywana podczas niszczenia obiektu sceny.
	/// </summary>
	void Destroy() {};
	/// <summary>
	/// Odœwie¿a dane siatki przesy³ane do wyœwietlenia.
	/// Stosowane po zmianie parametrów terenu.
	/// </summary>
	void RefreshMesh();

	/// <summary>
	/// Wywo³uje akcjê zapisu mapy wysokoœci terenu do pliku graficznego png.
	/// </summary>
	/// <param name="filename">wskaŸnik na nazwê pliku zapisu</param>
	void SaveHeightMapToImage(const char* filename);

	/// <summary>
	/// Zwraca wskaŸnik na tablicê z wartoœciami perm (ziarno generatora terenu).
	/// </summary>
	/// <returns>wskaŸnik na tablicê z wartoœciami perm (ziarno generatora terenu)</returns>
	unsigned char* GetSeedArray();

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
