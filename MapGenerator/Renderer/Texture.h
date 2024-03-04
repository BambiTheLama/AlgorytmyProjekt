#pragma once

#include "Renderer.h"

/// <summary>
/// Klasa reprezentuj¹ca teksturê.
/// </summary>
class Texture
{
public:
	/// <summary>
	/// Konstruktor przyjmuj¹cy œcie¿kê do pliku tekstury.
	/// </summary>
	/// <param name="path">referencja na obiekt z nazw¹ typu string</param>
	Texture(const std::string& path);
	~Texture();

	/// <summary>
	/// Metoda do wi¹zania tekstury na okreœlonym slocie OpenGL.
	/// </summary>
	/// <param name="slot"></param>
	void Bind(unsigned int slot = 0) const;
	/// <summary>
	/// Metoda do odwi¹zywania tekstury.
	/// </summary>
	void Unbind() const;

	/// <summary>
	/// Metoda zwracaj¹ca szerokoœæ tekstury.
	/// </summary>
	/// <returns></returns>
	inline int GetWidth() const { return m_Width; }
	/// <summary>
	/// Metoda zwracaj¹ca wysokoœæ tekstury.
	/// </summary>
	/// <returns></returns>
	inline int GetHeight() const { return m_Height; }

private:
	unsigned int m_RendererID; // Identyfikator renderera tekstury
	std::string m_FilePath; // Œcie¿ka do pliku tekstury
	unsigned char* m_LocalBuffer; // Lokalny bufor przechowuj¹cy dane tekstury
	int m_Width, m_Height, m_BPP; // Szerokoœæ, wysokoœæ i liczba bajtów na piksel tekstury
};