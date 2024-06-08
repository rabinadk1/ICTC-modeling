#pragma once

#include <GL/glew.h>

class VertexBuffer {
private:
  const GLuint m_RendererID;

public:
  // Data for the data of vertex buffer and size is the size of it
  VertexBuffer(const void *const data, const unsigned int size);

  // ~VertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

  inline void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

  inline void Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

private:
  inline static GLuint generateRendererID() {
    GLuint rendererID;
    glGenBuffers(1, &rendererID);
    return rendererID;
  }
};
