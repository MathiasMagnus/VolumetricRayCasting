
#include "Sphere.h"

Sphere::Sphere()
{
}

Sphere::Sphere(glm::vec3 center, float radius, float density, glm::vec3 color)
	:m_center(center), m_radius(radius), m_density(density), m_color(color)
{
	m_radius2 = m_radius * m_radius;
}
