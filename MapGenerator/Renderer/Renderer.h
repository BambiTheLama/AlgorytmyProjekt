#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// Makro ASSERT do sprawdzania warunków i debugowania
#define ASSERT(x) if (!(x)) __debugbreak();
#define GlCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)) // Makro GlCall do czyszczenia b³êdów i debugowania OpenGL, wypisuje w konsoli b³êdy OpenGL.

/// <summary>
///  Funkcja do czyszczenia b³êdów OpenGL.
/// </summary>
void GLClearError();
/// <summary>
/// Funkcja do logowania b³êdów OpenGL.
/// </summary>
/// <param name="function">wskaŸnik na nazwê funkcji</param>
/// <param name="file">wskaŸnik na nazwê pliku</param>
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
	/// Metoda do rysowania z wykorzystaniem tablicy wierzcho³ków, bufora indeksów i shadera.
	/// </summary>
	/// <param name="va">referencja na obiekt tablicy wierzcho³ków</param>
	/// <param name="ib">referencja na obiekt bufora indeksów</param>
	/// <param name="shader">referencja na obiekt shadera</param>
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	/// <summary>
	/// Metoda do rysowania z wykorzystaniem tablicy wierzcho³ków, liczby wierzcho³ków i shadera.
	/// Umo¿liwia rendering bez tablicy indeksów np. gdy ta nie jest dostêpna dla danej siatki modelu.
	/// </summary>
	/// <param name="va">referencja na obiekt tablicy wierzcho³ków</param>
	/// <param name="verticesNum">liczba wierzcho³ków</param>
	/// <param name="shader">referencja na obiekt shadera</param>
	void Draw(const VertexArray& va, const int verticesNum, const Shader& shader) const;

	/// <summary>
	/// Metoda do czyszczenia bufora ekranu.
	/// </summary>
	void Clear() const;

private:

};
