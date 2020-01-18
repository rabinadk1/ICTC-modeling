#include "Renderer.hpp"

// Changed by mouse movement
float Renderer::yaw = -90.f;
float Renderer::pitch = 0.f;

// Change by mouse scrolling
float Renderer::fov = 45.f;

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib)
{
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const VertexArray &va, uint vertexCount)
{
    va.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

// Callback function for printing debug statements
void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, const void *data)
{
    std::string _source, _type, _severity;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    default:
        _source = "UNKNOWN";
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
    }

    std::cerr << id << ": " << _type << " of " << _severity << " severity, raised from " << _source << " : " << msg << std::endl;
}

// glfw: whenever mouse position changes this function is executed
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float lastX = xpos, lastY = ypos;
    const float sensitivity = 0.05f;

    const float xoffset = (static_cast<float>(xpos) - lastX) * sensitivity;
    const float yoffset = (lastY - static_cast<float>(ypos)) * sensitivity; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    Renderer::yaw += xoffset;
    Renderer::pitch += yoffset;

    if (Renderer::pitch > 89.f)
        Renderer::pitch = 89.f;
    else if (Renderer::pitch < -89.f)
        Renderer::pitch = -89.f;
}

// glfw: whenever mouse wheel is scrolled this function is executed
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Renderer::fov -= yoffset;
    if (Renderer::fov < 1.f)
        Renderer::fov = 1.f;
    else if (Renderer::fov > 45.f)
        Renderer::fov = 45.f;
}

void processInput(GLFWwindow *window, glm::vec3 &cameraPos, const glm::vec3 &cameraFront, const glm::vec3 &cameraUp)
{
    // NOTE: The static variable initializes only once
    static float lastFrame = glfwGetTime(); // Time of last frame
    static float deltaTime;                 // Time between current frame and last frame

    const float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;
}
