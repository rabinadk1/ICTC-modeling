#pragma once
#include <string>

#include <GL/glew.h>

class Texture
{
private:
  unsigned int m_RendererID;
  const std::string m_Type;

public:
  Texture(const std::string &path, const char *type = "default");
  // ~Texture() { glDeleteTextures(1, &m_RendererID); }

  inline void Bind(unsigned int slot = 0) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
  }
  inline void Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
  inline const std::string &GetType() const { return m_Type; }
};
