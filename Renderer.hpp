#pragma once

#include <GL/glew.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

struct Renderer
{
    static inline void Clear() { glClear(GL_COLOR_BUFFER_BIT); }
    static void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);
};

// Callback function for printing debug statements
void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, const void *data);
