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
	/// Metoda wykonywana na pocz�tku sceny, po za�adowaniu.
	/// </summary>
	void Start() {};
	/// <summary>
	/// Metoda wykonywana w ka�dej p�tli aplikacji.
	/// </summary>
	void Update();
	/// <summary>
	/// Metoda wykonywana podczas niszczenia obiektu sceny.
	/// </summary>
	void Destroy() {};
	/// <summary>
	/// Od�wie�a dane siatki przesy�ane do wy�wietlenia.
	/// Stosowane po zmianie parametr�w terenu.
	/// </summary>
	void RefreshMesh();

	/// <summary>
	/// Wywo�uje akcj� zapisu mapy wysoko�ci terenu do pliku graficznego png.
	/// </summary>
	/// <param name="filename">wska�nik na nazw� pliku zapisu</param>
	void SaveHeightMapToImage(const char* filename);

	/// <summary>
	/// Zwraca wska�nik na tablic� z warto�ciami perm (ziarno generatora terenu).
	/// </summary>
	/// <returns>wska�nik na tablic� z warto�ciami perm (ziarno generatora terenu)</returns>
	unsigned char* GetSeedArray();

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
