#pragma once

#include <vector>
#include <cstddef>
#include "../Renderer/VertexArray.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/VertexBufferLayout.h"

/// <summary>
/// Klasa reprezentuj�ca siatk� (mesh) obiektu 3D.
/// </summary>
class Mesh {
private:

	std::vector<float> _vertices; // Wektor przechowuj�cy wierzcho�ki
	float* _verticesArray = nullptr; // Tablica wierzcho�k�w
	std::vector<unsigned int> _indices; // Wektor przechowuj�cy indeksy
	unsigned int* _indicesArray = nullptr; // Tablica indeks�w

	std::size_t IndexCount = 0; // Liczba indeks�w w siatce

	VertexArray va; // Obiekt klasy VertexArray
	IndexBuffer ib; // Obiekt klasy IndexBuffer
	VertexBuffer vb; // Obiekt klasy VertexBuffer
	VertexBufferLayout layout; // Obiekt klasy VertexBufferLayout

protected:
	Mesh();
	/// <summary>
	/// Metoda do ustawiania siatki.
	/// </summary>
	/// <param name="vertices">referencja do vectora wierzcho��w sitki</param>
	/// <param name="indices">referencja do vectora indeks�w wierzcho�k�w sitki</param>
	void SetMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	/// <summary>
	/// Metoda do aktualizacji wierzcho�k�w siatki.
	/// </summary>
	/// <param name="vertices">referencja do vectora wierzcho��w sitki</param>
	void UpdateVerticesMesh(const std::vector<float>& vertices);

public:

	/// <summary>
	/// Parametryczny konstruktor klasy Mesh.
	/// </summary>
	/// <param name="vertices">referencja do vectora wierzcho��w sitki</param>
	/// <param name="indices">referencja do vectora indeks�w wierzcho�k�w sitki</param>
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	/// <summary>
	/// Metoda zwracaj�ca referencj� do obiektu VertexArray
	/// </summary>
	/// <returns>referencja do obiektu VertexArray</returns>
	VertexArray& GetVa();
	/// <summary>
	/// Metoda zwracaj�ca referencj� do obiektu IndexBuffer
	/// </summary>
	/// <returns>referencja do obiektu IndexBuffer</returns>
	IndexBuffer& GetIb();

	/// <summary>
	/// Metoda do czyszczenia danych siatki.
	/// </summary>
	void Clear();

	/// <summary>
	/// Metoda zwracaj�ca liczb� tr�jk�t�w w siatce.
	/// </summary>
	/// <returns>liczba tr�jk�t�w w siatce</returns>
	auto GetTriangleCount() const noexcept { return IndexCount / 3; }
};