#pragma once
#include <string>
#include <vector>

#include <GL/glew.h>

#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "VertexArray.hpp"

class CubeMap {
private:
  const GLuint m_RendererID;

public:
  CubeMap(const std::vector<std::string> &faces);
  // ~Texture() { glDeleteTextures(1, &m_RendererID); }

  inline void Bind(const unsigned int slot = 0) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
  }

  void Draw(const VertexArray &va, const IndexBuffer &ib,
            const unsigned int slot = 0) const {
    // change depth function so depth test passes when values are equal to depth
    // buffer's content
    glDepthFunc(GL_LEQUAL);
    Bind(slot);
    Renderer::Draw(va, ib);
    // set depth function back to default
    glDepthFunc(GL_LESS);
  }

  void Draw(const VertexArray &va, unsigned int count,
            unsigned int slot = 0) const {
    // change depth function so depth test passes when values are equal to depth
    // buffer's content
    glDepthFunc(GL_LEQUAL);
    Bind(slot);
    Renderer::Draw(va, count);
    // set depth function back to default
    glDepthFunc(GL_LESS);
  }

  inline void Unbind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

private:
  inline static GLuint generateRenderedID() {
    GLuint renderedID;
    glGenTextures(1, &renderedID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, renderedID);
    return renderedID;
  }
};
