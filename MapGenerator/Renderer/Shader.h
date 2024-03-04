#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

/// <summary>
/// Struktura przechowuj¹ca Ÿród³a programu shadera.
/// </summary>
struct ShaderProgramSource
{
	std::string VertexSource; // ród³o shadera wierzcho³ków
	std::string GeometrySource; // ród³o shadera geometrii
	std::string FragmentSource; // ród³o shadera fragmentów
};

/// <summary>
/// Klasa reprezentuj¹ca shader GLSL dla OpenGL.
/// </summary>
class Shader
{
public:

	Shader();
	/// <summary>
	/// Konstruktor przyjmuj¹cy œcie¿kê do pliku shadera.
	/// </summary>
	/// <param name="filepath">referencja na œcie¿kê do pliku shadera obiektu typu string</param>
	Shader(const std::string& filepath);
	~Shader();

	/// <summary>
	/// Metoda do wi¹zania shadera.
	/// </summary>
	void Bind() const;
	/// <summary>
	/// Metoda do odwi¹zywania shadera.
	/// </summary>
	void Unbind() const;

	/// <summary>
	/// Metoda do ustawiania œcie¿ki do pliku shadera.
	/// </summary>
	/// <param name="filepath">referencja na œcie¿kê do pliku shadera obiektu typu string</param>
	void SetPath(const std::string& filepath);

	/// <summary>
	/// Metoda do ustawiania wartoœci uniformu typu int dla shadera.
	/// </summary>
	/// <param name="name">referencja na nazwê uniformu shadera</param>
	/// <param name="value">wartoœæ do przekazania shaderowi</param>
	void SetUniformi(const std::string& name, int value);
	/// <summary>
	/// Metoda do ustawiania wartoœci uniformu typu vec4.
	/// </summary>
	/// <param name="name">referencja na nazwê uniformu shadera</param>
	/// <param name="v0">pierwsza wartoœæ wektora vec4</param>
	/// <param name="v1">druga wartoœæ wektora vec4</param>
	/// <param name="v2">trzecia wartoœæ wektora vec4</param>
	/// <param name="v3">czwarta wartoœæ wektora vec4</param>
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	/// <summary>
	/// Metoda do ustawiania wartoœci uniformu typu vec3.
	/// </summary>
	/// <param name="name">referencja na nazwê uniformu shadera</param>
	/// <param name="v0">pierwsza wartoœæ wektora vec3</param>
	/// <param name="v1">druga wartoœæ wektora vec3</param>
	/// <param name="v2">trzecia wartoœæ wektora vec3</param>
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	/// <summary>
	/// Metoda do ustawiania wartoœci uniformu typu mat4.
	/// </summary>
	/// <param name="name">referencja na nazwê uniformu shadera</param>
	/// <param name="matrix">macie¿ glm::mat4</param>
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
private:
	unsigned int m_Renderer_ID; // Identyfikator renderera shadera

	std::string m_FilePath; // Œcie¿ka do pliku shadera
	std::unordered_map<std::string, int> m_UniformLocationCache; // Mapa do przechowywania lokalizacji uniformów

	/// <summary>
	/// Metoda do pobierania lokalizacji uniformu o podanej nazwie.
	/// </summary>
	/// <param name="name">referencja na nazwê uniformu shadera</param>
	/// <returns>identyfikator uniformu shadera</returns>
	unsigned int GetUniformLocation(const std::string& name);

	/// <summary>
	/// Metoda do parsowania Ÿróde³ shadera.
	/// </summary>
	/// <param name="filePath">referencja na œcie¿kê do pliku shadera obiektu typu string</param>
	/// <returns>struktura ShaderProgramSource reprezentuj¹ca obiekty tekstowe std::string ze Ÿród³ami shadera</returns>
	ShaderProgramSource ParseShader(const std::string& filePath);
	/// <summary>
	/// Metoda do kompilacji shadera o podanym typie i Ÿródle.
	/// </summary>
	/// <param name="type">typ shadera/ rodzaj (vertex, geometry, fragment)</param>
	/// <param name="source">referencja na obiekt typu string zawieraj¹cy tekst Ÿród³owy shadera</param>
	/// <returns>identyfikator shadera</returns>
	unsigned int CompilaShader(unsigned int type, const std::string& source);
	/// <summary>
	/// Metoda do tworzenia shadera z shaderów wierzcho³ków, fragmentów i geometrii.
	/// </summary>
	/// <param name="vertexShader">referencja na string ze Ÿród³em shadera wierzcho³ków</param>
	/// <param name="fragmentShader">referencja na string ze Ÿród³em shadera koloru</param>
	/// <param name="geometryShader">referencja na string ze Ÿród³em shadera geometrii</param>
	/// <returns>identyfikator ca³ego programu shadera z wszystkimi wczytanymi rodzajami shadera (vertex, geometry, fragment)</returns>
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
};