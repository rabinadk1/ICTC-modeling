#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
struct Renderer
{
    static float yaw, pitch, fov;

    static inline void Clear(uint clearBit = GL_COLOR_BUFFER_BIT) { glClear(clearBit); }
    static void Draw(const VertexArray &va, const IndexBuffer &ib);
    static void Draw(const VertexArray &va, uint vertexCount);
};

// Callback function for printing debug statements
void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, const void *data);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
inline void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, glm::vec3 &cameraPos, const glm::vec3 &cameraFront, const glm::vec3 &cameraUp);
