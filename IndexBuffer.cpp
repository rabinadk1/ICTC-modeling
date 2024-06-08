#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int *data, const unsigned int count)
    : m_Count(count), m_RendererID(generateRendererID()) {
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data,
               GL_STATIC_DRAW);
}
