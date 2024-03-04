#pragma once

#include <vector>
#include <cstddef>
#include "../Renderer/VertexArray.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/VertexBufferLayout.h"

/// <summary>
/// Klasa reprezentuj¹ca siatkê (mesh) obiektu 3D.
/// </summary>
class Mesh {
private:

	std::vector<float> _vertices; // Wektor przechowuj¹cy wierzcho³ki
	float* _verticesArray = nullptr; // Tablica wierzcho³ków
	std::vector<unsigned int> _indices; // Wektor przechowuj¹cy indeksy
	unsigned int* _indicesArray = nullptr; // Tablica indeksów

	std::size_t IndexCount = 0; // Liczba indeksów w siatce

	VertexArray va; // Obiekt klasy VertexArray
	IndexBuffer ib; // Obiekt klasy IndexBuffer
	VertexBuffer vb; // Obiekt klasy VertexBuffer
	VertexBufferLayout layout; // Obiekt klasy VertexBufferLayout

protected:
	Mesh();
	/// <summary>
	/// Metoda do ustawiania siatki.
	/// </summary>
	/// <param name="vertices">referencja do vectora wierzcho³ów sitki</param>
	/// <param name="indices">referencja do vectora indeksów wierzcho³ków sitki</param>
	void SetMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	/// <summary>
	/// Metoda do aktualizacji wierzcho³ków siatki.
	/// </summary>
	/// <param name="vertices">referencja do vectora wierzcho³ów sitki</param>
	void UpdateVerticesMesh(const std::vector<float>& vertices);

public:

	/// <summary>
	/// Parametryczny konstruktor klasy Mesh.
	/// </summary>
	/// <param name="vertices">referencja do vectora wierzcho³ów sitki</param>
	/// <param name="indices">referencja do vectora indeksów wierzcho³ków sitki</param>
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	/// <summary>
	/// Metoda zwracaj¹ca referencjê do obiektu VertexArray
	/// </summary>
	/// <returns>referencja do obiektu VertexArray</returns>
	VertexArray& GetVa();
	/// <summary>
	/// Metoda zwracaj¹ca referencjê do obiektu IndexBuffer
	/// </summary>
	/// <returns>referencja do obiektu IndexBuffer</returns>
	IndexBuffer& GetIb();

	/// <summary>
	/// Metoda do czyszczenia danych siatki.
	/// </summary>
	void Clear();

	/// <summary>
	/// Metoda zwracaj¹ca liczbê trójk¹tów w siatce.
	/// </summary>
	/// <returns>liczba trójk¹tów w siatce</returns>
	auto GetTriangleCount() const noexcept { return IndexCount / 3; }
};