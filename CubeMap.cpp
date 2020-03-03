#include <iostream>

#include "CubeMap.hpp"
#include "stb_image.h"

CubeMap::CubeMap(const std::vector<std::string> &faces)
{
  glGenTextures(1, &m_RendererID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

  int width, height, nrChannels;
  u_char *localBuffer;
  stbi_set_flip_vertically_on_load(false);
  for (uint i = 0; i < faces.size(); ++i)
  {
    /*
    load and generate the texture
    NOTE: Last argument to force the number of channels, **0 to not force**
    */
    localBuffer = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (localBuffer)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer);
      stbi_image_free(localBuffer);
    }
    else
      std::cerr << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
  }

  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Unbinding texture
  Unbind();
}
