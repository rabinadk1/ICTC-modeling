#pragma once

#include <GL/glew.h>

class IndexBuffer {
private:
  const unsigned int m_Count;
  const GLuint m_RendererID;

public:
  IndexBuffer(const unsigned int *const data, const unsigned int count);
  // ~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

  inline void Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  }
  inline void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };

  inline unsigned int GetCount() const { return m_Count; }

private:
  inline static GLuint generateRendererID() {
    GLuint rendererId;
    glGenBuffers(1, &rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
    return rendererId;
  }
};
