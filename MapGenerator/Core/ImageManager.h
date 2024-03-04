#pragma once

/// <summary>
/// Klasa pomocnicza udostępniająca funkcjonalność zapisu obrazu.
/// </summary>
class ImageManager
{
private:

public:

	/// <summary>
	/// Funkcja statyczna umożliwiająca na zapis obrazu z danych z tablicy do pliku typu png.
	/// </summary>
	/// <param name="filename">wskaźnik na nazwę pliku</param>
	/// <param name="width">szerokość obrazu</param>
	/// <param name="height">wysokość obrazu</param>
	/// <param name="image">wskaźnik na dane obrazu</param>
	static void SaveImage(const char* filename, int width, int height, const unsigned char* image);

	static bool LoadDataFromImageToArray(const char* filename, unsigned char* data);

};