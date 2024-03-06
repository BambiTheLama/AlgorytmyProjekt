#pragma once

#include "../Scene/TerrainParameters.h"

#include <string>

/// <summary>
/// Klasa udostêpniaj¹ca funkcjonalnoœæ zapisu i odczytu parametrów terenu.
/// </summary>
class DataParametersSaveManager {
private:

public:
	/// <summary>
	/// Funkcja zapisuj¹ca parametry terenu.
	/// </summary>
	/// <param name="filename">referencja na nazwê pliku</param>
	/// <param name="params">wskaŸnik na strukturê parametrów</param>
	/// <param name="seedArray">wskaŸnik na tablicê wartoœci danych ziarna</param>
	static void SaveParametersToFile(const std::string& filename, TerrainParameters* params, unsigned char* seedArray);

	/// <summary>
	/// Funkcja odczytuj¹ca parametry terenu.
	/// </summary>
	/// <param name="filename">referencja na nazwê pliku</param>
	/// <param name="params">wskaŸnik na strukturê parametrów</param>
	/// <param name="seedArray">wskaŸnik na tablicê wartoœci danych ziarna</param>
	/// <returns>true w przypadku powodzenia, false w przypadku b³êdu</returns>
	static bool LoadParametersFromFile(const std::string& filename, TerrainParameters* params, unsigned char* seedArray);
};