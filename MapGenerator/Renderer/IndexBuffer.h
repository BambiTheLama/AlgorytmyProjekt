#pragma once

/// <summary>
/// Klasa reprezentuj¹ca bufor indeksów OpenGL.
/// </summary>
class IndexBuffer
{
public:
	IndexBuffer();
	/// <summary>
	/// Konstruktor przyjmuj¹cy dane indeksów i liczbê indeksów.
	/// </summary>
	/// <param name="data">wskaŸnik na tablicê indeksów</param>
	/// <param name="count">rozmiar tablicy indeksów</param>
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	/// <summary>
	/// Metoda do ustawiania danych w buforze indeksów.
	/// </summary>
	/// <param name="data">wskaŸnik na tablicê indeksów</param>
	/// <param name="count">rozmiar tablicy indeksów</param>
	void Set(const unsigned int* data, unsigned int count);

	/// <summary>
	/// Metoda do wi¹zania bufora indeksów.
	/// </summary>
	void Bind() const;
	/// <summary>
	/// Metoda do odwi¹zywania bufora indeksów.
	/// </summary>
	void Unbind() const;

	/// <summary>
	/// Metoda zwracaj¹ca wskaŸnik na dane bufora indeksów.
	/// </summary>
	/// <returns>wskaŸnik na dane bufora indeksów</returns>
	const unsigned int* GetDataPtr() const;

	/// <summary>
	/// Metoda zwracaj¹ca liczbê indeksów w buforze.
	/// </summary>
	/// <returns>liczba indeksów w buforze</returns>
	inline unsigned int GetCount() const { return m_Count; }

private:
	unsigned int m_RendererID = 0; // Identyfikator renderera bufora indeksów
	unsigned int m_Count = 0; // Liczba indeksów w buforze

	const unsigned int* m_dataPtr = nullptr; // WskaŸnik na dane bufora indeksów
};
