#pragma once

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    const unsigned int m_Count;

public:
    IndexBuffer(const unsigned int *data, const unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_Count; }
};
