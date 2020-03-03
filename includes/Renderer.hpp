#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

struct Renderer
{
    static int w_width, w_height; // The size of window of glfw
    static Camera camera;         // Camera for rendering

    static inline void Clear(uint clearBit = GL_COLOR_BUFFER_BIT) { glClear(clearBit); }
    static void Draw(const VertexArray &va, const IndexBuffer &ib)
    {
        va.Bind();
        ib.Bind();

        glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    static void Draw(const VertexArray &va, uint vertexCount)
    {
        va.Bind();
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    static void EnableCullFace()
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
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
    Renderer::w_width = width;
    Renderer::w_height = height;
}

// glfw: whenever mouse wheel is scrolled this function is executed
inline void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Renderer::camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
