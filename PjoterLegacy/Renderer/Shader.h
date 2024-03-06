#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

/// <summary>
/// Struktura przechowuj�ca �r�d�a programu shadera.
/// </summary>
struct ShaderProgramSource
{
	std::string VertexSource; // �r�d�o shadera wierzcho�k�w
	std::string GeometrySource; // �r�d�o shadera geometrii
	std::string FragmentSource; // �r�d�o shadera fragment�w
};

/// <summary>
/// Klasa reprezentuj�ca shader GLSL dla OpenGL.
/// </summary>
class Shader
{
public:

	Shader();
	/// <summary>
	/// Konstruktor przyjmuj�cy �cie�k� do pliku shadera.
	/// </summary>
	/// <param name="filepath">referencja na �cie�k� do pliku shadera obiektu typu string</param>
	Shader(const std::string& filepath);
	~Shader();

	/// <summary>
	/// Metoda do wi�zania shadera.
	/// </summary>
	void Bind() const;
	/// <summary>
	/// Metoda do odwi�zywania shadera.
	/// </summary>
	void Unbind() const;

	/// <summary>
	/// Metoda do ustawiania �cie�ki do pliku shadera.
	/// </summary>
	/// <param name="filepath">referencja na �cie�k� do pliku shadera obiektu typu string</param>
	void SetPath(const std::string& filepath);

	/// <summary>
	/// Metoda do ustawiania warto�ci uniformu typu int dla shadera.
	/// </summary>
	/// <param name="name">referencja na nazw� uniformu shadera</param>
	/// <param name="value">warto�� do przekazania shaderowi</param>
	void SetUniformi(const std::string& name, int value);
	/// <summary>
	/// Metoda do ustawiania warto�ci uniformu typu vec4.
	/// </summary>
	/// <param name="name">referencja na nazw� uniformu shadera</param>
	/// <param name="v0">pierwsza warto�� wektora vec4</param>
	/// <param name="v1">druga warto�� wektora vec4</param>
	/// <param name="v2">trzecia warto�� wektora vec4</param>
	/// <param name="v3">czwarta warto�� wektora vec4</param>
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	/// <summary>
	/// Metoda do ustawiania warto�ci uniformu typu vec3.
	/// </summary>
	/// <param name="name">referencja na nazw� uniformu shadera</param>
	/// <param name="v0">pierwsza warto�� wektora vec3</param>
	/// <param name="v1">druga warto�� wektora vec3</param>
	/// <param name="v2">trzecia warto�� wektora vec3</param>
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	/// <summary>
	/// Metoda do ustawiania warto�ci uniformu typu mat4.
	/// </summary>
	/// <param name="name">referencja na nazw� uniformu shadera</param>
	/// <param name="matrix">macie� glm::mat4</param>
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
private:
	unsigned int m_Renderer_ID; // Identyfikator renderera shadera

	std::string m_FilePath; // �cie�ka do pliku shadera
	std::unordered_map<std::string, int> m_UniformLocationCache; // Mapa do przechowywania lokalizacji uniform�w

	/// <summary>
	/// Metoda do pobierania lokalizacji uniformu o podanej nazwie.
	/// </summary>
	/// <param name="name">referencja na nazw� uniformu shadera</param>
	/// <returns>identyfikator uniformu shadera</returns>
	unsigned int GetUniformLocation(const std::string& name);

	/// <summary>
	/// Metoda do parsowania �r�de� shadera.
	/// </summary>
	/// <param name="filePath">referencja na �cie�k� do pliku shadera obiektu typu string</param>
	/// <returns>struktura ShaderProgramSource reprezentuj�ca obiekty tekstowe std::string ze �r�d�ami shadera</returns>
	ShaderProgramSource ParseShader(const std::string& filePath);
	/// <summary>
	/// Metoda do kompilacji shadera o podanym typie i �r�dle.
	/// </summary>
	/// <param name="type">typ shadera/ rodzaj (vertex, geometry, fragment)</param>
	/// <param name="source">referencja na obiekt typu string zawieraj�cy tekst �r�d�owy shadera</param>
	/// <returns>identyfikator shadera</returns>
	unsigned int CompilaShader(unsigned int type, const std::string& source);
	/// <summary>
	/// Metoda do tworzenia shadera z shader�w wierzcho�k�w, fragment�w i geometrii.
	/// </summary>
	/// <param name="vertexShader">referencja na string ze �r�d�em shadera wierzcho�k�w</param>
	/// <param name="fragmentShader">referencja na string ze �r�d�em shadera koloru</param>
	/// <param name="geometryShader">referencja na string ze �r�d�em shadera geometrii</param>
	/// <returns>identyfikator ca�ego programu shadera z wszystkimi wczytanymi rodzajami shadera (vertex, geometry, fragment)</returns>
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
};