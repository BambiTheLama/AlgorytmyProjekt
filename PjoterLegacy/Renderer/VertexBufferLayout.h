#pragma once

#include <vector>

#include "renderer.h"

/// <summary>
/// Struktura reprezentuj�ca element bufora wierzcho�k�w.
/// Reprezentuje nast�puj�ce dane:
/// "unsigned int count" - Liczba element�w w buforze
/// "unsigned int type" - Typ elementu (np. GL_FLOAT, GL_UNSIGNED_INT, GL_UNSIGNED_BYTE)
/// "unsigned char normalized" - Czy warto�ci s� znormalizowane
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
		ASSERT(false); // Wywo�anie asercji w przypadku nieznanego typu elementu
		return 0;
	}
};

/// <summary>
/// Klasa reprezentuj�ca uk�ad bufora wierzcho�k�w.
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
	/// Szablonowa metoda Push do dodawania element�w do bufora.
	/// Oblicza r�wnie� krok (offset) mi�dzy kolejnymi elementami bufora dla dodanego elementu.
	/// </summary>
	/// <typeparam name="t">dost�pne dla typ�w: float, unsigned int i unsigned char</typeparam>
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
	/// Metoda zwracaj�ca wektor element�w bufora wierzcho�k�w.
	/// </summary>
	/// <returns></returns>
	inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
	/// <summary>
	/// Metoda zwracaj�ca krok (offset) mi�dzy kolejnymi elementami bufora.
	/// </summary>
	/// <returns></returns>
	inline unsigned int GetStride() const { return m_Stride; }
};