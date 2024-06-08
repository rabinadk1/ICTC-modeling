#pragma once

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {
private:
  const GLuint m_RendererID;

public:
  VertexArray() : m_RendererID(generateRendererID()) {}
  VertexArray(const VertexBuffer &vb, const VertexBufferLayout &layout)
      : m_RendererID(generateRendererID()) {
    AddBuffer(vb, layout);
  }
  // ~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

  void AddBuffer(const VertexBuffer &vb,
                 const VertexBufferLayout &layout) const;

  inline void Bind() const { glBindVertexArray(m_RendererID); }

  inline void Unbind() const { glBindVertexArray(0); }

private:
  inline static GLuint generateRendererID() {
    GLuint rendererID;
    glGenVertexArrays(1, &rendererID);
    return rendererID;
  }
};
