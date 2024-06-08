#include "Camera.hpp"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, const float yaw,
               const float pitch, const float fov)
    : m_Yaw(yaw), m_Pitch(pitch), m_FOV(fov), m_Up(up), m_Position(position),
      m_Front(calculateFrontVector(yaw, pitch)),
      m_Right(calculateRightVector(m_Front, up)) {}

Camera::Camera(const float posX, const float posY, const float posZ,
               const float upX, const float upY, const float upZ,
               const float yaw, const float pitch, const float fov)
    : m_Yaw(yaw), m_Pitch(pitch), m_FOV(fov), m_Up(glm::vec3(upX, upY, upZ)),
      m_Position(glm::vec3(posX, posY, posZ)),
      m_Front(calculateFrontVector(yaw, pitch)),
      m_Right(calculateRightVector(m_Front, m_Up)) {}

void Camera::ProcessKeyboard(const CameraMovement &direction,
                             const float deltaTime) {
  constexpr float plane_y = -1.f;

  const float velocity = 2.5f * deltaTime;

  switch (direction) {
  case CameraMovement::FORWARD:
    m_Position += m_Front * velocity;
    break;
  case CameraMovement::BACKWARD:
    m_Position -= m_Front * velocity;
    break;
  case CameraMovement::LEFT:
    m_Position -= m_Right * velocity;
    break;
  case CameraMovement::RIGHT:
    m_Position += m_Right * velocity;
    break;
  case CameraMovement::UP:
    m_Position += m_Up * velocity;
    break;
  // For CameraMovement::DOWN
  default:
    m_Position -= m_Up * velocity;
  }
  if (m_Position.y <= plane_y)
    m_Position.y = plane_y;
}

void Camera::ProcessMouseMovement(const float xoffset, const float yoffset,
                                  bool constrainPitch) {
  constexpr float mouseSensitivity = 0.1f;
  m_Yaw += xoffset * mouseSensitivity;
  m_Pitch += yoffset * mouseSensitivity;

  constexpr float max_Pitch = 89.f;
  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (m_Pitch > max_Pitch)
      m_Pitch = max_Pitch;
    else if (m_Pitch < -max_Pitch)
      m_Pitch = -max_Pitch;
  }

  // Update Front & Right Vectors using the updated Euler angles

  // Calculate the new Front vector
  m_Front = calculateFrontVector(m_Yaw, m_Pitch);

  /*
  Recalculate the right vector also
  Normalize the vectors, because their length gets closer to 0 the more you look
  up or down which results in slower movement.
  */
  m_Right = calculateRightVector(m_Front, m_Up);
}

void Camera::ProcessMouseScroll(const float yoffset) {
  m_FOV -= -yoffset;
  if (m_FOV < 1.f)
    m_FOV = 1.f;
  else if (m_FOV > 45.f)
    m_FOV = 45.f;
}
