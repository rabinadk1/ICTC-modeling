#include "VertexArray.hpp"

void VertexArray::AddBuffer(const VertexBuffer &vb,
                            const VertexBufferLayout &layout) const {
  Bind();
  vb.Bind();
  const auto elements = layout.GetElements();
  unsigned int offset = 0;
  for (size_t i = 0; i < elements.size(); ++i) {
    const VertexBufferElement &element = elements[i];
    glVertexAttribPointer(i, element.count, element.type, element.normalized,
                          layout.GetStride(), reinterpret_cast<void *>(offset));
    glEnableVertexAttribArray(i);
    offset += element.count * element.GetTypeSize();
  }
}
