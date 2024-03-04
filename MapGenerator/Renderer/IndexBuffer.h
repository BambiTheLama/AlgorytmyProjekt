#pragma once

/// <summary>
/// Klasa reprezentuj�ca bufor indeks�w OpenGL.
/// </summary>
class IndexBuffer
{
public:
	IndexBuffer();
	/// <summary>
	/// Konstruktor przyjmuj�cy dane indeks�w i liczb� indeks�w.
	/// </summary>
	/// <param name="data">wska�nik na tablic� indeks�w</param>
	/// <param name="count">rozmiar tablicy indeks�w</param>
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	/// <summary>
	/// Metoda do ustawiania danych w buforze indeks�w.
	/// </summary>
	/// <param name="data">wska�nik na tablic� indeks�w</param>
	/// <param name="count">rozmiar tablicy indeks�w</param>
	void Set(const unsigned int* data, unsigned int count);

	/// <summary>
	/// Metoda do wi�zania bufora indeks�w.
	/// </summary>
	void Bind() const;
	/// <summary>
	/// Metoda do odwi�zywania bufora indeks�w.
	/// </summary>
	void Unbind() const;

	/// <summary>
	/// Metoda zwracaj�ca wska�nik na dane bufora indeks�w.
	/// </summary>
	/// <returns>wska�nik na dane bufora indeks�w</returns>
	const unsigned int* GetDataPtr() const;

	/// <summary>
	/// Metoda zwracaj�ca liczb� indeks�w w buforze.
	/// </summary>
	/// <returns>liczba indeks�w w buforze</returns>
	inline unsigned int GetCount() const { return m_Count; }

private:
	unsigned int m_RendererID = 0; // Identyfikator renderera bufora indeks�w
	unsigned int m_Count = 0; // Liczba indeks�w w buforze

	const unsigned int* m_dataPtr = nullptr; // Wska�nik na dane bufora indeks�w
};
