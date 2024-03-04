#pragma once

/// <summary>
/// Klasa pomocnicza udostêpniaj¹ca funkcjonalnoœæ zapisu obrazu.
/// </summary>
class ImageManager
{
private:

public:

	/// <summary>
	/// Funkcja statyczna umo¿liwiaj¹ca na zapis obrazu z danych z tablicy do pliku typu png.
	/// </summary>
	/// <param name="filename">wskaŸnik na nazwê pliku</param>
	/// <param name="width">szerokoœæ obrazu</param>
	/// <param name="height">wysokoœæ obrazu</param>
	/// <param name="image">wskaŸnik na dane obrazu</param>
	static void SaveImage(const char* filename, int width, int height, const unsigned char* image);

	static bool LoadDataFromImageToArray(const char* filename, unsigned char* data);

};