#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader {
private:
  // For Debug Purposes
  const std::string m_Filepath;

  const unsigned int m_RendererID;

  std::unordered_map<const char *, int> m_UniformLocationCache;

public:
  Shader(const char *const filepath);
  ~Shader() { glDeleteProgram(m_RendererID); }

  inline void Bind() const { glUseProgram(m_RendererID); }
  inline void Unbind() const { glUseProgram(0); }

  // Set uniform of given name
  inline void SetUniform(const char *const name, const int value) {
    glUniform1i(getUniformLocation(name), value);
  }

  inline void SetUniform(const char *const name, const float value) {
    glUniform1f(getUniformLocation(name), value);
  }

  inline void SetUniform(const char *const name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, false,
                       glm::value_ptr(matrix));
  }

  inline void SetUniform(const char *const name, const float v0, const float v1,
                         const float v2, const float v3) {
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
  }

  inline void SetUniform(const char *const name, const float v0, const float v1,
                         const float v2) {
    glUniform3f(getUniformLocation(name), v0, v1, v2);
  }

  inline void SetUniform(const char *const name, const glm::vec3 &vector) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(vector));
  }

private:
  /*
  Parses the file in **filepath** and return source code of
  vertex shader and fragment shader as pair
  */
  static std::pair<std::string, std::string>
  parseShader(const char *const filepath);

  // Compiles shaders from the source code
  inline static GLuint compileShader(const uint type,
                                     const std::string &source) {
    return compileShader(type, source.c_str());
  }
  static GLuint compileShader(const uint type, const char *const source);

  // Compiles both shaders and return a program containing both
  inline static GLuint createShader(const std::string &vertexShader,
                                    const std::string &fragmentShader) {
    return createShader(vertexShader.c_str(), fragmentShader.c_str());
  }
  static GLuint createShader(const char *const vertexShader,
                             const char *const fragmentShader);

  // Gets the location of provided uniform
  GLint getUniformLocation(const char *const name);

  inline static GLuint generateRendererID(const char *const filepath) {
    // Get sources of vertex and fragment shader
    const auto source = parseShader(filepath);

    // Get program with vertex and fragent shader linked to it
    return createShader(source.first, source.second);
  }
};
