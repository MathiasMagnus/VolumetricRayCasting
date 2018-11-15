#pragma once

// GLM includes
#ifdef _MSC_VER 
#pragma warning( push )
#pragma warning( disable : 4127 ) /* Prevents warning about conditional expression is constant */
#pragma warning( disable : 4201 ) /* Prevents warning about nonstandard extension used: nameless struct/union */
#endif
#include <glm/glm.hpp>
#ifdef _MSC_VER 
#pragma warning( pop )
#endif

class Sphere
{
public:
	Sphere() = default;
	Sphere(glm::vec3 center, float radius, float density, glm::vec3 color)
        : m_center(center)
        , m_radius(radius)
        , m_radius2(m_radius * m_radius)
        , m_density(density)
        , m_color(color)
    {}

	glm::vec3 GetCenter() { return m_center; }
	void SetCenter(glm::vec3 center) { m_center = center;}
	void SetRadius(float radius) { m_radius = radius; m_radius2 = m_radius * m_radius; }
	float GetRadius() { return m_radius; }
	float GetDensity() { return m_density; }
	glm::vec3 GetColor() { return m_color; }
	bool GetIntersections(const glm::vec3 &rayorig, const glm::vec3 &raydir, float &t0, float &t1) const {
		glm::vec3 l = m_center - rayorig;
		float tca = glm::dot(l, raydir);
		float d2 = glm::dot(l, l) - tca * tca;

		bool isIntersected = true;
		if ((m_radius2 - d2) < 0.0001f)
			isIntersected = false;

		float thc = sqrt(m_radius2 - d2);
		t0 = tca - thc;
		t1 = tca + thc;

		return isIntersected;

		return true;
	}

	glm::vec3 m_center;
	float m_radius;
	float m_radius2;
	float m_density;
	glm::vec3 m_color;

};
