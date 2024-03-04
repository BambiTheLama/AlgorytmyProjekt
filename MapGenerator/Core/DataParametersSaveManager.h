#pragma once

#include "../Scene/TerrainParameters.h"

#include <string>

/// <summary>
/// Klasa udost�pniaj�ca funkcjonalno�� zapisu i odczytu parametr�w terenu.
/// </summary>
class DataParametersSaveManager {
private:

public:
	/// <summary>
	/// Funkcja zapisuj�ca parametry terenu.
	/// </summary>
	/// <param name="filename">referencja na nazw� pliku</param>
	/// <param name="params">wska�nik na struktur� parametr�w</param>
	/// <param name="seedArray">wska�nik na tablic� warto�ci danych ziarna</param>
	static void SaveParametersToFile(const std::string& filename, TerrainParameters* params, unsigned char* seedArray);

	/// <summary>
	/// Funkcja odczytuj�ca parametry terenu.
	/// </summary>
	/// <param name="filename">referencja na nazw� pliku</param>
	/// <param name="params">wska�nik na struktur� parametr�w</param>
	/// <param name="seedArray">wska�nik na tablic� warto�ci danych ziarna</param>
	/// <returns>true w przypadku powodzenia, false w przypadku b��du</returns>
	static bool LoadParametersFromFile(const std::string& filename, TerrainParameters* params, unsigned char* seedArray);
};