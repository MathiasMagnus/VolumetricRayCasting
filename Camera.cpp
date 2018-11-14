#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "Camera.h"
Camera::Camera(glm::vec3 position, glm::vec3 viewedPos, glm::vec3 viewUp, float fieldOfView)
:m_position(position), m_viewedPosition(viewedPos), m_viewUp(viewUp), m_fieldOfView(fieldOfView)
{
	m_position = position;
	m_viewedPosition = viewedPos;
	m_viewDir = glm::vec3(m_viewedPosition - m_position);
	m_viewDir = glm::normalize(m_viewDir);
	m_viewUp = viewUp;
	m_fieldOfView = fieldOfView; // in degrees

	// rotation matrix with 10 degree
	float rotateWithDegree = glm::radians(10.f);
	//float rotateWithDegree = 90;
	rotatorAroundXClock = glm::rotate(-rotateWithDegree, glm::vec3(0.1f, 0.0f, 0.0f)); 
	rotatorAroundYClock = glm::rotate(-rotateWithDegree, glm::vec3(0.0f, 1.0f, 0.0f));
	rotatorAroundZClock = glm::rotate(-rotateWithDegree, glm::vec3(0.0f, 0.0f, 0.1f));

	rotatorAroundXCClock = glm::rotate(rotateWithDegree, glm::vec3(0.1f, 0.0f, 0.0f));
	rotatorAroundYCClock = glm::rotate(rotateWithDegree, glm::vec3(0.0f, 1.0f, 0.0f));
	rotatorAroundZCClock = glm::rotate(rotateWithDegree, glm::vec3(0.0f, 0.0f, 0.1f));

}

//Orthonormalizes camera view direction, view up, and view right vectors
bool Camera::Orthonormalize()
{
	// making a right-handed coordinate system
	m_viewDir = glm::normalize(m_viewDir);

	m_viewSide = glm::cross(m_viewUp, m_viewDir); // right vector
	m_viewSide = glm::normalize(m_viewSide);

	m_viewUp = glm::cross(m_viewDir, m_viewSide);
	m_viewUp = glm::normalize(m_viewUp);
	return true;
}

void Camera::SetPosition(const glm::vec3 &cameraPos)
{
	m_position = cameraPos;
}


void Camera::RotateAroundXClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundXClock * m_position;
	m_position += m_viewedPosition;

	m_viewUp = rotatorAroundXClock * m_viewUp;
	m_viewUp = glm::normalize(m_viewUp);
}

void Camera::RotateAroundXCClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundXCClock * m_position;
	m_position += m_viewedPosition;

	m_viewUp = rotatorAroundXCClock * m_viewUp;
	m_viewUp = glm::normalize(m_viewUp);
}
void Camera::RotateAroundYClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundYClock * m_position;
	m_position += m_viewedPosition;

	m_viewUp = rotatorAroundYClock * m_viewUp;
	m_viewUp = glm::normalize(m_viewUp);
}

void Camera::RotateAroundYCClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundYCClock * m_position;
	m_position += m_viewedPosition;

	m_viewUp = rotatorAroundYCClock * m_viewUp;
	m_viewUp = glm::normalize(m_viewUp);
}

void Camera::RotateAroundZClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundZClock * m_position;
	m_position += m_viewedPosition;
}

void Camera::RotateAroundZCClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundZCClock * m_position;
	m_position += m_viewedPosition;
}
