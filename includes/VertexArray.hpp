#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
private:
    unsigned int m_RendererID;

public:
    VertexArray() { glGenVertexArrays(1, &m_RendererID); }
    // ~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) const;

    inline void Bind() const { glBindVertexArray(m_RendererID); }

    inline void Unbind() const { glBindVertexArray(0); }
};
