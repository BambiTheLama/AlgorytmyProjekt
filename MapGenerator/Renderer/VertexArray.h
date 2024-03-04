#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

/// <summary>
/// Klasa reprezentuj�ca tablic� wierzcho�k�w OpenGL.
/// </summary>
class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	/// <summary>
	/// Metoda do wi�zania tablicy wierzcho�k�w.
	/// </summary>
	void Bind() const;
	/// <summary>
	/// Metoda do odwi�zywania tablicy wierzcho�k�w.
	/// </summary>
	void Unbind() const;
	/// <summary>
	/// Metoda do dodawania bufora wierzcho�k�w i uk�adu bufora wierzcho�k�w do tablicy.
	/// </summary>
	/// <param name="vb">referencja do obiektu bufora wierzcho�k�w</param>
	/// <param name="layout">referencja do obiektu uk�adu bufora wierzcho�k�w</param>
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
private:
	unsigned int m_RendererID; // Identyfikator renderera tablicy wierzcho�k�w
};