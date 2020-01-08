#pragma once
#include <string>

#include <GL/glew.h>

class Texture
{
private:
  unsigned int m_RendererID;
  std::string m_Filepath;
  int m_Width, m_Height, m_ChannelNumber;

public:
  Texture(const char *path);
  ~Texture() { glDeleteTextures(1, &m_RendererID); }

  inline void Bind(unsigned int slot = 0) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
  }
  inline void Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
};
