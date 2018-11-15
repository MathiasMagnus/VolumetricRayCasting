#pragma once

// GLM includes
#include <glm/ext.hpp>

class Camera
{
public:
	Camera() {};
	
	Camera(glm::vec3 position, glm::vec3 viewedPos, glm::vec3 viewUp, float fieldOfView);


	bool Orthonormalize();

	glm::vec3 GetPosition() const { return m_position; }
	glm::vec3 GetViewDir() const  { return m_viewDir; }
	glm::vec3 GetViewUp() const  { return m_viewUp; }
	glm::vec3 GetViewSide() const { return m_viewSide; }
	glm::vec3 GetViewedPos() const { return m_viewedPosition; }
	void RotateAroundXClock();
	void RotateAroundYClock();
	void RotateAroundZClock();

	void RotateAroundXCClock();
	void RotateAroundYCClock();
	void RotateAroundZCClock();

	void SetPosition(const glm::vec3 &cameraPos);

	glm::vec3 m_position;
	glm::vec3 m_viewDir;
	glm::vec3 m_viewUp;
	glm::vec3 m_viewSide;
	glm::vec3 m_viewedPosition;
	float	m_fieldOfView;

	glm::mat3 rotatorAroundXClock;
	glm::mat3 rotatorAroundYClock;
	glm::mat3 rotatorAroundZClock;

	glm::mat3 rotatorAroundXCClock;
	glm::mat3 rotatorAroundYCClock;
	glm::mat3 rotatorAroundZCClock;
};
