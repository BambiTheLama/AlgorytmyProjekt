#pragma once

/// <summary>
/// Klasa reprezentuj¹ca bufor wierzcho³ków OpenGL.
/// </summary>
class VertexBuffer
{
public:
	VertexBuffer();
	/// <summary>
	/// Konstruktor przyjmuj¹cy dane i rozmiar bufora.
	/// </summary>
	/// <param name="data">wskaŸnik na tablicê danych</param>
	/// <param name="size">rozmiar tablicy</param>
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	/// <summary>
	/// Metoda do ustawiania danych w buforze.
	/// </summary>
	/// <param name="data">wskaŸnik na nowe dane</param>
	/// <param name="size">rozmiar nowych danych</param>
	void Set(const void* data, unsigned int size);

	/// <summary>
	/// Metoda do wi¹zania bufora.
	/// Potrzebna podczas renderingu danych podanej tablicy wierzcho³ków.
	/// </summary>
	void Bind() const;
	/// <summary>
	/// Metoda do odwi¹zywania bufora.
	/// Potrzebna po renderingu danych podanej tablicy wierzcho³ków.
	/// </summary>
	void Unbind() const;

private:
	unsigned int m_RendererID; // Identyfikator renderera bufora
};
