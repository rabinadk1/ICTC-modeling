#include "Camera.hpp"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, float yaw, float pitch)
    : m_Position(position),
      m_Up(up),
      m_Yaw(yaw),
      m_Pitch(pitch),
      m_FOV(45.f)
{
  updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : m_Position(glm::vec3(posX, posY, posZ)),
      m_Up(glm::vec3(upX, upY, upZ)),
      m_Yaw(yaw),
      m_Pitch(pitch),
      m_FOV(45.f)
{
  updateCameraVectors();
}

void Camera::ProcessKeyboard(const CameraMovement &direction, float deltaTime)
{
  const float velocity = 2.5f * deltaTime;

  switch (direction)
  {
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
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
  const float mouseSensitivity = 0.1f;
  m_Yaw += xoffset * mouseSensitivity;
  m_Pitch += yoffset * mouseSensitivity;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (m_Pitch > 89.0f)
      m_Pitch = 89.0f;
    else if (m_Pitch < -89.0f)
      m_Pitch = -89.0f;
  }

  // Update Front & Right Vectors using the updated Euler angles
  updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
  m_FOV -= yoffset;
  if (m_FOV < 1.f)
    m_FOV = 1.f;
  else if (m_FOV > 45.f)
    m_FOV = 45.f;
}

void Camera::updateCameraVectors()
{
  // Calculate the new Front vector
  m_Front = glm::normalize(
      glm::vec3(
          cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
          sin(glm::radians(m_Pitch)),
          sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))));

  /*
  Recalculate the right vector also
  Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  */
  m_Right = glm::normalize(glm::cross(m_Front, m_Up));
}
