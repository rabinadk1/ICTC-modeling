#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

#include <GL/glew.h>

struct VertexBufferElement
{
    unsigned int type;
    // Number of floats defining the position
    unsigned int count;
    /*
    GL_TRUE is 1 and
    GL_FALSE is 0
    */
    bool normalized;

    // Returns the size of given type
    unsigned int GetTypeSize() const
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(float);
        case GL_UNSIGNED_INT:
            return sizeof(unsigned int);
        case GL_UNSIGNED_BYTE:
            return sizeof(unsigned char);
        default:
            throw std::invalid_argument("Type is not among float, unsigned int or unsigned char");
        }
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout() : m_Stride(0) {}

    template <typename>
    inline void Push(unsigned int count)
    {
        std::cerr << "Invalid buffer layout type" << std::endl;
        exit(1);
    }

    inline const std::vector<VertexBufferElement> &GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};

template <>
inline void VertexBufferLayout::Push<float>(unsigned int count)
{
    m_Elements.push_back({GL_FLOAT, count, false});
    m_Stride += sizeof(float) * count;
}

template <>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({GL_UNSIGNED_INT, count, false});
    m_Stride += sizeof(unsigned int) * count;
}

template <>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, true});
    m_Stride += sizeof(unsigned char) * count;
}
