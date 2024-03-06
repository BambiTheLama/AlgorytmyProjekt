#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// Makro ASSERT do sprawdzania warunk�w i debugowania
#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)) // Makro GlCall do czyszczenia b��d�w i debugowania OpenGL, wypisuje w konsoli b��dy OpenGL.

/// <summary>
///  Funkcja do czyszczenia b��d�w OpenGL.
/// </summary>
void GLClearError();
/// <summary>
/// Funkcja do logowania b��d�w OpenGL.
/// </summary>
/// <param name="function">wska�nik na nazw� funkcji</param>
/// <param name="file">wska�nik na nazw� pliku</param>
/// <param name="line">numer linii</param>
/// <returns></returns>
bool GLLogCall(const char* function, const char* file, int line);

/// <summary>
/// Klasa odpowiedzialna za renderowanie.
/// </summary>
class Renderer
{
public:
	/// <summary>
	/// Metoda do rysowania z wykorzystaniem tablicy wierzcho�k�w, bufora indeks�w i shadera.
	/// </summary>
	/// <param name="va">referencja na obiekt tablicy wierzcho�k�w</param>
	/// <param name="ib">referencja na obiekt bufora indeks�w</param>
	/// <param name="shader">referencja na obiekt shadera</param>
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	/// <summary>
	/// Metoda do rysowania z wykorzystaniem tablicy wierzcho�k�w, liczby wierzcho�k�w i shadera.
	/// Umo�liwia rendering bez tablicy indeks�w np. gdy ta nie jest dost�pna dla danej siatki modelu.
	/// </summary>
	/// <param name="va">referencja na obiekt tablicy wierzcho�k�w</param>
	/// <param name="verticesNum">liczba wierzcho�k�w</param>
	/// <param name="shader">referencja na obiekt shadera</param>
	void Draw(const VertexArray& va, const int verticesNum, const Shader& shader) const;

	/// <summary>
	/// Metoda do czyszczenia bufora ekranu.
	/// </summary>
	void Clear() const;

private:

};
