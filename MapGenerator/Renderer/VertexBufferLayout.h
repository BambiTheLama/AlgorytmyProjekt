#pragma once

#include <vector>

#include "renderer.h"

/// <summary>
/// Struktura reprezentuj¹ca element bufora wierzcho³ków.
/// Reprezentuje nastêpuj¹ce dane:
/// "unsigned int count" - Liczba elementów w buforze
/// "unsigned int type" - Typ elementu (np. GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_BYTE)
/// "unsigned char normalized" - Czy wartoœci s¹ znormalizowane
/// </summary>
struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:          return 4;
			case GL_UNSIGNED_INT:   return 4;
			case GL_UNSIGNED_BYTE:  return 1;
		}
		ASSERT(false); // Wywo³anie asercji w przypadku nieznanego typu elementu
		return 0;
	}
};

/// <summary>
/// Klasa reprezentuj¹ca uk³ad bufora wierzcho³ków.
/// </summary>
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {};
	~VertexBufferLayout() {};

	/// <summary>
	/// Szablonowa metoda Push do dodawania elementów do bufora.
	/// Oblicza równie¿ krok (offset) miêdzy kolejnymi elementami bufora dla dodanego elementu.
	/// </summary>
	/// <typeparam name="t">dostêpne dla typów: float, unsigned int i unsigned char</typeparam>
	/// <param name="count"></param>
	template<typename t>
	void Push(unsigned int count) = delete;
	
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_FLOAT, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	/// <summary>
	/// Metoda zwracaj¹ca wektor elementów bufora wierzcho³ków.
	/// </summary>
	/// <returns></returns>
	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
	/// <summary>
	/// Metoda zwracaj¹ca krok (offset) miêdzy kolejnymi elementami bufora.
	/// </summary>
	/// <returns></returns>
	inline unsigned int GetStride() const { return m_Stride; }
};