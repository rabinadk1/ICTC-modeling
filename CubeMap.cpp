#include <iostream>

#include "CubeMap.hpp"
#include "stb_image.h"

CubeMap::CubeMap(const std::vector<std::string> &faces)
    : m_RendererID(generateRenderedID()) {

  stbi_set_flip_vertically_on_load(false);
  for (size_t i = 0; i < faces.size(); ++i) {
    int width, height, nrChannels;
    /*
    load and generate the texture
    NOTE: Last argument to force the number of channels, **0 to not force**
    */
    if (const auto localBuffer =
            stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        localBuffer) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer);
      stbi_image_free(localBuffer);
    } else
      std::cerr << "Cubemap tex failed to load at path: " << faces[i]
                << std::endl;
  }

  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Unbinding texture
  Unbind();
}
