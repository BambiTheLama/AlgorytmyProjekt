#pragma once

#include "Renderer.h"

/// <summary>
/// Klasa reprezentuj�ca tekstur�.
/// </summary>
class Texture
{
public:
	/// <summary>
	/// Konstruktor przyjmuj�cy �cie�k� do pliku tekstury.
	/// </summary>
	/// <param name="path">referencja na obiekt z nazw� typu string</param>
	Texture(const std::string& path);
	~Texture();

	/// <summary>
	/// Metoda do wi�zania tekstury na okre�lonym slocie OpenGL.
	/// </summary>
	/// <param name="slot"></param>
	void Bind(unsigned int slot = 0) const;
	/// <summary>
	/// Metoda do odwi�zywania tekstury.
	/// </summary>
	void Unbind() const;

	/// <summary>
	/// Metoda zwracaj�ca szeroko�� tekstury.
	/// </summary>
	/// <returns></returns>
	inline int GetWidth() const { return m_Width; }
	/// <summary>
	/// Metoda zwracaj�ca wysoko�� tekstury.
	/// </summary>
	/// <returns></returns>
	inline int GetHeight() const { return m_Height; }

private:
	unsigned int m_RendererID; // Identyfikator renderera tekstury
	std::string m_FilePath; // �cie�ka do pliku tekstury
	unsigned char* m_LocalBuffer; // Lokalny bufor przechowuj�cy dane tekstury
	int m_Width, m_Height, m_BPP; // Szeroko��, wysoko�� i liczba bajt�w na piksel tekstury
};