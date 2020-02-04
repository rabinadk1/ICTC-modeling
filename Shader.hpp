#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
private:
    // For Debug Purposes
    const std::string m_Filepath;

    unsigned int m_RendererID;

    std::unordered_map<const char *, int> m_UniformLocationCache;

public:
    Shader(const char *filepath);
    ~Shader() { glDeleteProgram(m_RendererID); }

    inline void Bind() const { glUseProgram(m_RendererID); }
    inline void Unbind() const { glUseProgram(0); }

    // Set uniform of given name
    inline void SetUniform(const char *name, int value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    inline void SetUniform(const char *name, float value)
    {
        glUniform1f(GetUniformLocation(name), value);
    }

    inline void SetUniform(const char *name, const glm::mat4 &matrix)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix));
    }

    inline void SetUniform(const char *name, float v0, float v1, float v2, float v3)
    {
        glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
    }

    inline void SetUniform(const char *name, float v0, float v1, float v2)
    {
        glUniform3f(GetUniformLocation(name), v0, v1, v2);
    }

    inline void SetUniform(const char *name, const glm::vec3 &vector)
    {
        glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vector));
    }

private:
    /*
    Parses the file in **filepath** and return source code of
    vertex shader and fragment shader as pair
    */
    std::pair<std::string, std::string> ParseShader(const char *filepath) const;

    // Compiles shaders from the source code
    uint CompileShader(const uint type, const std::string &source) const;

    // Compiles both shaders and return a program containing both
    uint CreateShader(const std::string &vertexShader, const std::string &fragmentShader) const;

    // Gets the location of provided uniform
    int GetUniformLocation(const char *name);
};
