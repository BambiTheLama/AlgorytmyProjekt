#pragma once

/// <summary>
/// Klasa reprezentuj�ca bufor wierzcho�k�w OpenGL.
/// </summary>
class VertexBuffer
{
public:
	VertexBuffer();
	/// <summary>
	/// Konstruktor przyjmuj�cy dane i rozmiar bufora.
	/// </summary>
	/// <param name="data">wska�nik na tablic� danych</param>
	/// <param name="size">rozmiar tablicy</param>
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	/// <summary>
	/// Metoda do ustawiania danych w buforze.
	/// </summary>
	/// <param name="data">wska�nik na nowe dane</param>
	/// <param name="size">rozmiar nowych danych</param>
	void Set(const void* data, unsigned int size);

	/// <summary>
	/// Metoda do wi�zania bufora.
	/// Potrzebna podczas renderingu danych podanej tablicy wierzcho�k�w.
	/// </summary>
	void Bind() const;
	/// <summary>
	/// Metoda do odwi�zywania bufora.
	/// Potrzebna po renderingu danych podanej tablicy wierzcho�k�w.
	/// </summary>
	void Unbind() const;

private:
	unsigned int m_RendererID; // Identyfikator renderera bufora
};
