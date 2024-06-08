#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera {
private:
  // Euler Angles
  float m_Yaw, m_Pitch;

  // Camera Option i.e. field of vision
  float m_FOV;

  // Camera Attributes
  const glm::vec3 m_Up;
  glm::vec3 m_Position, m_Front, m_Right;

public:
  // Constructor with vectors
  Camera(const glm::vec3 &position = glm::vec3(0.f, 0.f, 0.f),
         const glm::vec3 &up = glm::vec3(0.f, 1.f, 0.f),
         const float yaw = -90.f, const float pitch = 0.f,
         const float fov = 45.f);

  // Constructor with scalar values
  Camera(const float posX, const float posY, const float posZ, const float upX,
         const float upY, const float upZ, const float yaw = -90.f,
         const float pitch = 0.f, const float fov = 45.f);

  // Processes input received from any keyboard-like input system. Accepts input
  // parameter in the form of camera defined ENUM (to abstract it from windowing
  // systems)
  void ProcessKeyboard(const CameraMovement &direction, const float deltaTime);

  // Processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void ProcessMouseMovement(const float xoffset, const float yoffset,
                            const bool constrainPitch = true);

  // Processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset);

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  inline glm::mat4 GetViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
  }

  inline float GetFOV() const { return m_FOV; }
  inline const glm::vec3 &GetPosition() const { return m_Position; }

private:
  static glm::vec3 calculateFrontVector(float yaw, float pitch) {
    const auto rad_yaw = glm::radians(yaw);
    const auto rad_pitch = glm::radians(pitch);
    return glm::normalize(glm::vec3(cos(rad_yaw) * cos(rad_pitch),
                                    sin(rad_pitch),
                                    sin(rad_yaw) * cos(rad_pitch)));
  }

  static inline glm::vec3 calculateRightVector(glm::vec3 front, glm::vec3 up) {
    return glm::normalize(glm::cross(front, up));
  }
};
