#ifndef INCLUDED_CAMERA_H
#define INCLUDED_CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#ifndef EPSILON
#define EPSILON 0.001
#endif

class Camera
{
public:
	Camera();
	Camera(QVector3D position, QVector3D viewDir, QVector3D viewUp, QVector3D viewSide);

	bool Orthonormalize();

	QVector3D GetPosition() const { return m_position; }
	QVector3D GetViewDir() const  { return m_viewDir; }
	QVector3D GetViewUp() const  { return m_viewUp; }
	QVector3D GetViewSide() const { return m_viewSide; }
	QVector3D GetViewedPos() const { return m_viewedPosition; }
	void RotateAroundXClock();
	void RotateAroundYClock();
	void RotateAroundZClock();

	void RotateAroundXCClock();
	void RotateAroundYCClock();
	void RotateAroundZCClock();

	void SetPosition(const QVector3D &cameraPos);

	QVector3D m_position;
	QVector3D m_viewDir;
	QVector3D m_viewUp;
	QVector3D m_viewSide;
	QVector3D m_viewedPosition;
	float	m_fieldOfView;

	QMatrix4x4 rotatorAroundXClock;
	QMatrix4x4 rotatorAroundYClock;
	QMatrix4x4 rotatorAroundZClock;

	QMatrix4x4 rotatorAroundXCClock;
	QMatrix4x4 rotatorAroundYCClock;
	QMatrix4x4 rotatorAroundZCClock;


};

#endif