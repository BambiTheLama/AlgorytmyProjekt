#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

/// <summary>
/// Klasa reprezentuj¹ca tablicê wierzcho³ków OpenGL.
/// </summary>
class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	/// <summary>
	/// Metoda do wi¹zania tablicy wierzcho³ków.
	/// </summary>
	void Bind() const;
	/// <summary>
	/// Metoda do odwi¹zywania tablicy wierzcho³ków.
	/// </summary>
	void Unbind() const;
	/// <summary>
	/// Metoda do dodawania bufora wierzcho³ków i uk³adu bufora wierzcho³ków do tablicy.
	/// </summary>
	/// <param name="vb">referencja do obiektu bufora wierzcho³ków</param>
	/// <param name="layout">referencja do obiektu uk³adu bufora wierzcho³ków</param>
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
private:
	unsigned int m_RendererID; // Identyfikator renderera tablicy wierzcho³ków
};