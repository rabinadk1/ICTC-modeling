#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void *data, const unsigned int size)
{
    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
