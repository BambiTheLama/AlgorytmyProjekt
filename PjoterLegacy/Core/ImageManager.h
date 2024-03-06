#pragma once

/// <summary>
/// Klasa pomocnicza udost�pniaj�ca funkcjonalno�� zapisu obrazu.
/// </summary>
class ImageManager
{
private:

public:

	/// <summary>
	/// Funkcja statyczna umo�liwiaj�ca na zapis obrazu z danych z tablicy do pliku typu png.
	/// </summary>
	/// <param name="filename">wska�nik na nazw� pliku</param>
	/// <param name="width">szeroko�� obrazu</param>
	/// <param name="height">wysoko�� obrazu</param>
	/// <param name="image">wska�nik na dane obrazu</param>
	static void SaveImage(const char* filename, int width, int height, const unsigned char* image);

	static bool LoadDataFromImageToArray(const char* filename, unsigned char* data);

};