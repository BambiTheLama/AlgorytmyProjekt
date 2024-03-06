#include "Shader.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "renderer.h"

Shader::Shader() { m_Renderer_ID = 0; }

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_Renderer_ID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_Renderer_ID = CreateShader(source.VertexSource, source.FragmentSource, source.GeometrySource);
}

Shader::~Shader()
{
    GlCall(glDeleteProgram(m_Renderer_ID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
    };

    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("geometry") != std::string::npos)
                type = ShaderType::GEOMETRY;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[2].str(), ss[1].str() };
}

void Shader::SetPath(const std::string& filepath)
{
    m_FilePath = filepath;
    m_Renderer_ID = 0;
    ShaderProgramSource source = ParseShader(filepath);
    m_Renderer_ID = CreateShader(source.VertexSource, source.FragmentSource, source.GeometrySource);
}

unsigned int Shader::CompilaShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile: ";
            if (type == GL_VERTEX_SHADER) {
                std::cout << "Vertex" << std::endl;
            }
            else if (type == GL_GEOMETRY_SHADER) {
                std::cout << "Geometry" << std::endl;
            }
            else if (type == GL_FRAGMENT_SHADER) {
                std::cout << "Fragment" << std::endl;
            }
        std::cout << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompilaShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int gs = (!geometryShader.empty()) ? CompilaShader(GL_GEOMETRY_SHADER, geometryShader) : 0;
    unsigned int fs = CompilaShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    if (!geometryShader.empty())
        glAttachShader(program, gs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    if (!geometryShader.empty())
        glDeleteShader(gs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const
{
    GlCall(glUseProgram(m_Renderer_ID));
}

void Shader::Unbind() const
{
    GlCall(glUseProgram(0));
}

void Shader::SetUniformi(const std::string& name, int value)
{
    GlCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GlCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    GlCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
    GlCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GlCall(int location = glGetUniformLocation(m_Renderer_ID, name.c_str()));

    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;

	return location;
}
