#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.hpp"

Shader::Shader(const char *filepath)
    : m_Filepath(filepath)
{
    // Get sources of vertex and fragment shader
    std::pair<std::string, std::string> source = ParseShader(filepath);

    // Get program with vertex and fragent shader linked to it
    m_RendererID = CreateShader(source.first, source.second);
}

std::pair<std::string, std::string> Shader::ParseShader(const char *filepath) const
{
    enum class ShaderType
    {
        NONE = -1,
        VERTEX,
        FRAGMENT
    };
    std::ifstream stream(filepath);
    if (!stream)
    {
        std::cerr << "Cannot read " << filepath << std::endl;
        exit(1);
    }
    std::string line;
    // One for vertex shader and other for fragment shader
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.rfind("#shader", 0) != std::string::npos)
            // The length of "#shader" is 7
            type = line.rfind("vertex", 8) != std::string::npos ? ShaderType::VERTEX : ShaderType::FRAGMENT;
        else
            ss[static_cast<int>(type)] << line << "\n";
    }

    return {ss[0].str(), ss[1].str()};
}

uint Shader::CompileShader(const uint type, const std::string &source) const
{
    const uint id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        std::cerr << "ERROR:Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

uint Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) const
{
    const uint program = glCreateProgram();
    const uint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    const uint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int Shader::GetUniformLocation(const char *name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name);

    // Making sure **name** is found in shader
    if (location == -1)
        std::cerr << "Warning: Uniform " << name << " doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}
