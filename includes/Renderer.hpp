#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "Camera.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

struct Renderer {
  static int w_width, w_height; // The size of window of glfw
  static Camera camera;         // Camera for rendering

  static inline void Clear(const unsigned int clearBit = GL_COLOR_BUFFER_BIT) {
    glClear(clearBit);
  }

  static void Draw(const VertexArray &va, const IndexBuffer &ib) {
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
  }

  static void Draw(const VertexArray &va, const unsigned int vertexCount) {
    va.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
  }

  static void EnableCullFace() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
};

// Callback function for printing debug statements
void GLDebugMessageCallback(const GLenum source, const GLenum type,
                            const GLuint id, const GLenum severity,
                            const GLsizei length, const GLchar *const msg,
                            const void *const data);

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
inline void framebuffer_size_callback(GLFWwindow *const, const int width,
                                      const int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
  Renderer::w_width = width;
  Renderer::w_height = height;
}

// glfw: whenever mouse wheel is scrolled this function is executed
inline void scroll_callback(GLFWwindow *const, double, const double yoffset) {
  Renderer::camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow *const window, const double xpos,
                    const double ypos);
void processInput(GLFWwindow *const window);
