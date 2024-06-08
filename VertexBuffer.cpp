#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void *data, const unsigned int size)
    : m_RendererID(generateRendererID()) {
  Bind();
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
