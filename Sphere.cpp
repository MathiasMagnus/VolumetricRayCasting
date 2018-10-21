
#include "Sphere.h"

Sphere::Sphere()
{
}

Sphere::Sphere(QVector3D center, float radius, float density, QVector3D color)
:m_center(center), m_radius(radius), m_density(density), m_color(color)
{
	m_radius2 = m_radius * m_radius;
}

bool Sphere::GetIntersections(const QVector3D &rayorig, const QVector3D &raydir, float &t0, float &t1) const
{
	QVector3D l = m_center - rayorig;
	float tca = QVector3D::dotProduct(l, raydir);
	float d2 = QVector3D::dotProduct(l, l) - tca * tca;
	
	if (d2 > m_radius2) 
		return false;
	
	float thc = sqrt(m_radius2 - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	return true;
}