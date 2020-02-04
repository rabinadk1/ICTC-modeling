#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

class Camera
{
private:
  // Camera Attributes
  glm::vec3 m_Position, m_Front, m_Right;
  const glm::vec3 m_Up;

  // Euler Angles
  float m_Yaw, m_Pitch;

  // Camera Option i.e. field of vision
  float m_FOV;

public:
  // Constructor with vectors
  Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0.f);

  // Constructor with scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = -90.f, float pitch = 0.f);

  // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
  void ProcessKeyboard(const CameraMovement &direction, float deltaTime);

  // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset);

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  inline glm::mat4 GetViewMatrix() const
  {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
  }

  inline float GetFOV() const { return m_FOV; }
  inline const glm::vec3 &GetPosition() const { return m_Position; }

private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors();
};
