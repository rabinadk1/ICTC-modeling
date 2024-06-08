#include <fstream>
#include <iostream>
#include <sstream>

#include "Shader.hpp"

Shader::Shader(const char *filepath)
    : m_Filepath(filepath), m_RendererID(generateRendererID(filepath)),
      m_UniformLocationCache() {}

std::pair<std::string, std::string>
Shader::parseShader(const char *const filepath) {
  enum class ShaderType { NONE = -1, VERTEX, FRAGMENT };

  std::ifstream stream(filepath);
  if (!stream) {
    std::cerr << "Cannot read " << filepath << std::endl;
    exit(1);
  }
  std::string line;
  // One for vertex shader and other for fragment shader
  std::stringstream ss[2];

  ShaderType type = ShaderType::NONE;

  while (getline(stream, line)) {
    if (line.rfind("#shader", 0) != std::string::npos)
      // The length of "#shader" is 7
      type = line.rfind("vertex", 8) != std::string::npos
                 ? ShaderType::VERTEX
                 : ShaderType::FRAGMENT;
    else
      ss[static_cast<int>(type)] << line << "\n";
  }
  return {ss[0].str(), ss[1].str()};
}

GLuint Shader::compileShader(const uint type, const char *source) {
  const auto id = glCreateShader(type);
  glShaderSource(id, 1, &source, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (!result) {
    std::cerr << "ERROR:Failed to compile "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader"
              << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

GLuint Shader::createShader(const char *const vertexShader,
                            const char *const fragmentShader) {
  const auto program = glCreateProgram();
  const auto vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  const auto fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

GLint Shader::getUniformLocation(const char *const name) {
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  const auto location = glGetUniformLocation(m_RendererID, name);

  // Making sure **name** is found in shader
  if (location == -1)
    std::cerr << "Warning: Uniform " << name << " doesn't exist!" << std::endl;

  m_UniformLocationCache[name] = location;
  return location;
}
