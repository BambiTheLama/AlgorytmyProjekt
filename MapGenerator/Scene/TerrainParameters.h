#pragma once

/// <summary>
/// Struktura okreœlaj¹ca parametry terenu.
/// </summary>
struct TerrainParameters {
	float scale; // Skala terenu
	float persistence; // Wspó³czynnik utrzymywania szumów
	int octaves; // Liczba oktaw
	int height; // Wysokoœæ terenu
	float lacunarity; // Zwielokrotnienie, okreœla zmianê skali miêdzy kolejnymi oktawami
	float exponentiation; // Wspó³czynnik potêgowania
	bool regenerateTerrain; // Flaga okreœlaj¹ca potrzebê regeneracji terenu
};