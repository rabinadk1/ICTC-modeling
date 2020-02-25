#pragma once

#include <GL/glew.h>

class VertexBuffer
{
private:
    unsigned int m_RendererID;

public:
    // Data for the data of vertex buffer and size is the size of it
    VertexBuffer(const void *data, const unsigned int size);

    // ~VertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    inline void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

    inline void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};
