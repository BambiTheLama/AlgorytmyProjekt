#pragma once

/// <summary>
/// Struktura okre�laj�ca parametry terenu.
/// </summary>
struct TerrainParameters {
	float scale; // Skala terenu
	float persistence; // Wsp�czynnik utrzymywania szum�w
	int octaves; // Liczba oktaw
	int height; // Wysoko�� terenu
	float lacunarity; // Zwielokrotnienie, okre�la zmian� skali mi�dzy kolejnymi oktawami
	float exponentiation; // Wsp�czynnik pot�gowania
	bool regenerateTerrain; // Flaga okre�laj�ca potrzeb� regeneracji terenu
};