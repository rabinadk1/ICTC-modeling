#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    const unsigned int m_Count;

public:
    IndexBuffer(const unsigned int *data, const unsigned int count);
    // ~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

    inline void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }
    inline void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };

    inline unsigned int GetCount() const { return m_Count; }
};
