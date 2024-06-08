#include "Renderer.hpp"

// Changed by resizing
int Renderer::w_width = 1366;
int Renderer::w_height = 768;

// Camera
Camera Renderer::camera(glm::vec3(31.1f, -.3f, -9.4f),
                        glm::vec3(0.0f, 1.0f, 0.0f), -200.f, 10.f);

static const char *getDebugSource(GLenum source) {
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    return "API";

  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    return "WINDOW SYSTEM";

  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    return "SHADER COMPILER";

  case GL_DEBUG_SOURCE_THIRD_PARTY:
    return "THIRD PARTY";

  case GL_DEBUG_SOURCE_APPLICATION:
    return "APPLICATION";

  default:
    return "UNKNOWN";
  }
}

static const char *getDebugType(GLenum type) {
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    return "ERROR";

  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    return "DEPRECATED BEHAVIOR";

  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    return "UDEFINED BEHAVIOR";

  case GL_DEBUG_TYPE_PORTABILITY:
    return "PORTABILITY";

  case GL_DEBUG_TYPE_PERFORMANCE:
    return "PERFORMANCE";

  case GL_DEBUG_TYPE_OTHER:
    return "OTHER";

  case GL_DEBUG_TYPE_MARKER:
    return "MARKER";

  default:
    return "UNKNOWN";
  }
}

static const char *getDeubgSeverity(GLenum severity) {
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    return "HIGH";

  case GL_DEBUG_SEVERITY_MEDIUM:
    return "MEDIUM";

  case GL_DEBUG_SEVERITY_LOW:
    return "LOW";

  case GL_DEBUG_SEVERITY_NOTIFICATION:
    return "NOTIFICATION";

  default:
    return "UNKNOWN";
  }
}

// Callback function for printing debug statements
void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei, const GLchar *msg,
                            const void *) {

  const auto _source = getDebugSource(source);
  const auto _type = getDebugType(type);
  const auto _severity = getDeubgSeverity(severity);

  std::cerr << id << ": " << _type << " of " << _severity
            << " severity, raised from " << _source << " : " << msg
            << std::endl;
}

// glfw: whenever mouse position changes this function is executed
void mouse_callback(GLFWwindow *const, double xpos, double ypos) {
  static float lastX = xpos, lastY = ypos;

  // reversed yoffset since y-coordinates range from bottom to top
  Renderer::camera.ProcessMouseMovement(xpos - lastX, lastY - ypos);

  lastX = xpos;
  lastY = ypos;
}

void processInput(GLFWwindow *const window) {
  // NOTE: The static variable initializes only once
  static float lastFrame = glfwGetTime(); // Time of last frame
  static float deltaTime; // Time between current frame and last frame

  const float currentTime = glfwGetTime();
  deltaTime = currentTime - lastFrame;
  lastFrame = currentTime;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    Renderer::camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    Renderer::camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    Renderer::camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    Renderer::camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    Renderer::camera.ProcessKeyboard(CameraMovement::UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    Renderer::camera.ProcessKeyboard(CameraMovement::DOWN, deltaTime);
}
