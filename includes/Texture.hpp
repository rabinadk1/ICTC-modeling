#pragma once
#include <string>

#include <GL/glew.h>

class Texture {
private:
  const std::string m_Type;
  const GLuint m_RendererID;

public:
  Texture(const std::string &path, const char *const type = "default");
  // ~Texture() { glDeleteTextures(1, &m_RendererID); }

  inline void Bind(const unsigned int slot = 0) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
  }
  inline void Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
  inline const std::string &GetType() const { return m_Type; }

private:
  inline static GLuint generateRendererID() {
    GLuint rendererID;
    glGenTextures(1, &rendererID);
    glBindTexture(GL_TEXTURE_2D, rendererID);
    return rendererID;
  }
};
