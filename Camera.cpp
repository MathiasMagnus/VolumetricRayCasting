#include "Camera.h"

Camera::Camera(QVector3D position, QVector3D viewedPos, QVector3D viewUp, float fieldOfView)
:m_position(position), m_viewedPosition(viewedPos), m_viewUp(viewUp), m_fieldOfView(fieldOfView)
{
	m_position = position;
	m_viewedPosition = viewedPos;
	m_viewDir = QVector3D(m_viewedPosition - m_position);
	m_viewDir.normalize();
	m_viewUp = viewUp;
	m_fieldOfView = fieldOfView; // in degrees

	// rotation matrix with 10 degree
	float rotateWithDegree = 10;
	//float rotateWithDegree = 90;
	rotatorAroundXClock.rotate(-rotateWithDegree, 0.1, 0.0, 0.0);
	rotatorAroundYClock.rotate(-rotateWithDegree, 0.0, 1.0, 0.0);
	rotatorAroundZClock.rotate(-rotateWithDegree, 0.0, 0.0, 0.1);

	rotatorAroundXCClock.rotate(rotateWithDegree, 0.1, 0.0, 0.0);
	rotatorAroundYCClock.rotate(rotateWithDegree, 0.0, 1.0, 0.0);
	rotatorAroundZCClock.rotate(rotateWithDegree, 0.0, 0.0, 0.1);

}

//Orthonormalizes camera view direction, view up, and view right vectors
bool Camera::Orthonormalize()
{
	// making a right-handed coordinate system
	m_viewDir.normalize();

	m_viewSide = QVector3D::crossProduct(m_viewUp, m_viewDir); // right vector
	m_viewSide.normalize();

	m_viewUp = QVector3D::crossProduct(m_viewDir, m_viewSide);
	m_viewUp.normalize();
	return true;
}

void Camera::SetPosition(const QVector3D &cameraPos)
{
	m_position = cameraPos;
}


void Camera::RotateAroundXClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundXClock * m_position;
	m_position += m_viewedPosition;

	m_viewUp = rotatorAroundXClock * m_viewUp;
	m_viewUp.normalize();
}

void Camera::RotateAroundXCClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundXCClock * m_position;
	m_position += m_viewedPosition;

	m_viewUp = rotatorAroundXCClock * m_viewUp;
	m_viewUp.normalize();
}
void Camera::RotateAroundYClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundYClock * m_position;
	m_position += m_viewedPosition;

	m_viewUp = rotatorAroundYClock * m_viewUp;
	m_viewUp.normalize();
}

void Camera::RotateAroundYCClock()
{
	m_position -= m_viewedPosition;
	m_position = rotatorAroundYCClock * m_position;
	m_position += m_viewedPosition;

	m_viewUp = rotatorAroundYCClock * m_viewUp;
	m_viewUp.normalize();
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
