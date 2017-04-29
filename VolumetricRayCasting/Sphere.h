#ifndef INCLUDED_SPHERE_H
#define INCLUDED_SPHERE_H

#include <QVector3D>
#include <iostream>
#include <fstream>
#include <sstream>

class Sphere
{
public:
	Sphere();
	Sphere(QVector3D center, float radius, float density, QVector3D color);

	QVector3D GetCenter() { return m_center; }
	void SetCenter(QVector3D center) { m_center = center;}
	void SetRadius(float radius) { m_radius = radius; m_radius2 = m_radius * m_radius; }
	float GetRadius() { return m_radius; }
	float GetDensity() { return m_density; }
	QVector3D GetColor() { return m_color; }
	bool GetIntersections(const QVector3D &rayorig, const QVector3D &raydir, float &t0, float &t1);

private:
	QVector3D m_center;
	float m_radius;
	float m_radius2;
	float m_density;
	QVector3D m_color;

};

#endif