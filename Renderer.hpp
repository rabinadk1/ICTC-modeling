#pragma once

#include <GL/glew.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
struct Renderer
{
    static inline void Clear(uint clearBit = GL_COLOR_BUFFER_BIT) { glClear(clearBit); }
    static void Draw(const VertexArray &va, const IndexBuffer &ib);
    static void Draw(const VertexArray &va, uint vertexCount);
};

// Callback function for printing debug statements
void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, const void *data);
