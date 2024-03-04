#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

/// <summary>
/// Klasa reprezentująca tablicę wierzchołków OpenGL.
/// </summary>
class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	/// <summary>
	/// Metoda do wiązania tablicy wierzchołków.
	/// </summary>
	void Bind() const;
	/// <summary>
	/// Metoda do odwiązywania tablicy wierzchołków.
	/// </summary>
	void Unbind() const;
	/// <summary>
	/// Metoda do dodawania bufora wierzchołków i układu bufora wierzchołków do tablicy.
	/// </summary>
	/// <param name="vb">referencja do obiektu bufora wierzchołków</param>
	/// <param name="layout">referencja do obiektu układu bufora wierzchołków</param>
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
private:
	unsigned int m_RendererID; // Identyfikator renderera tablicy wierzchołków
};