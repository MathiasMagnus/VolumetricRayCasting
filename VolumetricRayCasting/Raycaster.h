#ifndef INCLUDED_SCENE_H
#define INCLUDED_SCENE_H

#include <functional>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Sphere.h"
#include "Camera.h"

template <typename DensFunc, typename ColorFunc>
class Raycaster
{
public:
	Raycaster(DensFunc densFunc, ColorFunc colorFunc) : m_densityFunc(densFunc), m_colorFunc(colorFunc)
	{
		m_saturationThreshold = 0;
	}

	void operator()(QImage& image, int imageWidth, int imageHeight, const  Camera& camera)
	{
		Raycast(image, imageWidth, imageHeight, camera);
	}

	//Initiate the volume rendering of the scene by raycasting/raymarching for each pixel
	void Raycast(QImage& inImage, int imageWidth, int imageHeight, const Camera& camera)
	{
		//std::ofstream ofs("bitNr.txt");
		auto bytePointer = inImage.bits();
		int byteCount = 0;

		CreateTransformMatrices(camera);
	
		float aspectRatio = (float)imageWidth / (float)imageHeight;
		float x, y, z;

		z = -1.0f;

		float scaleFOV = tan(camera.m_fieldOfView / 2 * M_PI / 180);

		for (unsigned int i = 0; i < imageHeight; i++)
		{
			y = (1 - 2 * (i + 0.5) / (float)imageHeight) * scaleFOV;
			for (unsigned int j = 0; j < imageWidth; j++)
			{
				x = (2 * (j + 0.5) / (float)imageWidth - 1) * aspectRatio * scaleFOV;
				QVector3D rayVec(x, y, z);

				float t0 = -1E+36;
				float t1 = -1E+36;

				/*
				// test ray
				rayVec.SetX(0.0);
				rayVec.SetY(0.0);
				rayVec.SetZ(-1.0);
				*/

				TransformToWorldCoordinates(rayVec);
				QVector3D transformedCamRayDir = rayVec - camera.GetPosition();
				transformedCamRayDir.normalize();
				bool bIntersected = m_sphere.GetIntersections(camera.GetPosition(), transformedCamRayDir, t0, t1);

				QColor pixelColor;
				if (bIntersected && t0 > 0.0 && t1 > 0.0)
				{
					pixelColor = Raymarch(camera.GetPosition(), transformedCamRayDir, t0, t1);
				}
				// if we are inside the spehere, we trace from the the ray's original position
				else if (bIntersected && t1 > 0.0)
				{
					pixelColor = Raymarch(camera.GetPosition(), transformedCamRayDir, 0.0, t1);
				}
				else
				{
					pixelColor = QColor(0, 0, 0);
				}

				// seting rgb value for every pixel
				bytePointer[byteCount] = pixelColor.red();
				byteCount++;
				bytePointer[byteCount] = pixelColor.green();
				byteCount++;
				bytePointer[byteCount] = pixelColor.blue();
				byteCount++;
				//ofs << byteCount << std::endl;
			}
		}
	}

	//std::ofstream out("C:/Users/balaz/Desktop/degree_atan2.txt");
	//Raymarch from startT to endT, accumulating density
	QColor Raymarch(const QVector3D& camPos, const QVector3D& rayDirection, float startT, float endT)
	{
		QColor finalColor(0, 0, 0, 0);
		QVector3D location(0.0f, 0.0f, 0.0f);

		location = camPos + startT*rayDirection;

		float current_t = startT;
		QVector4D accumulatedDensity;

		while (current_t < endT)
		{
			location = location + m_deltaS*rayDirection;
			current_t += m_deltaS;
			if (!IsOutside(location))
			{
				// Convert to spherical coordinated
				float r = sqrt(location.x()*location.x() + location.y()*location.y() + location.z()*location.z());
				float theta = acos(location.z() / r); //* 180 / M_PI; // convert to degrees?
				float phi = atan2(location.y(), location.x()); //* 180 / M_PI;
														
				QColor color = m_colorFunc(m_densityFunc(r, theta, phi));
				
				finalColor.setRed(finalColor.red() + color.red());
				finalColor.setGreen(finalColor.green() + color.green());
				finalColor.setBlue(finalColor.blue() + color.blue());
			}

			// stop the ray, when color reaches the saturation.
			if (finalColor.red() > m_saturationThreshold || finalColor.green() > m_saturationThreshold 
				|| finalColor.blue() > m_saturationThreshold)
				break;
		}

		// normalizer according to the highest rgb value
		auto normalizer = std::max(1, std::max(std::max(finalColor.red(), finalColor.green()), finalColor.blue()));
		finalColor.setRed(finalColor.red() / normalizer * 255);
		finalColor.setGreen(finalColor.green() / normalizer * 255);
		finalColor.setBlue(finalColor.blue() / normalizer * 255);

		return finalColor;
	}

	void SetLimits(const std::array<std::array<float, 2>, 3 > &extent)
	{
		m_minX = extent[0][0];
		m_maxX = extent[0][1];

		m_minY = extent[1][0];
		m_maxY = extent[1][1];

		m_minZ = extent[2][0];
		m_maxZ = extent[2][1];

		if (m_minX > m_maxX || m_minY > m_maxY || m_minZ > m_maxZ)
			throw std::runtime_error("Error: higher limit should be bigger than the lower!");

		// find center of the encapsulating sphere
		m_sphere.SetCenter(QVector3D((m_minX + m_maxX) / 2, (m_minY + m_maxY) / 2, (m_minZ + m_maxZ) / 2));
		float maxRadius = std::max({ (m_maxX - m_minX) , (m_maxY - m_minY) , (m_maxZ - m_minZ) }) / 2;
		m_sphere.SetRadius(maxRadius*1.8);

		m_deltaS = std::min({ (m_maxX - m_minX) , (m_maxY - m_minY) , (m_maxZ - m_minZ) }) / 100; //  step size
	}

	void CreateTransformMatrices(const Camera& camera)
	{
		m_worldToViewMtx.setToIdentity();
		m_ViewToWorldMtx.setToIdentity();
		m_worldToViewMtx.lookAt(camera.m_position, camera.m_viewedPosition, camera.m_viewUp);
		m_ViewToWorldMtx = m_worldToViewMtx.inverted(); // is this always invertible? if not, than wut?
	}

	void TransformToViewCoordinates(QVector3D & worldToViewCoord) const
	{
		QVector4D  worldToViewCoord4(worldToViewCoord.x(), worldToViewCoord.y(), worldToViewCoord.z(), 1.0f);
		worldToViewCoord4 = m_worldToViewMtx * worldToViewCoord4;
		worldToViewCoord = QVector3D(worldToViewCoord4.x(), worldToViewCoord4.y(), worldToViewCoord4.z());
	}

	void TransformToWorldCoordinates(QVector3D & viewToWorldCoord) const
	{
		QVector4D  viewToWorldCoord4(viewToWorldCoord.x(), viewToWorldCoord.y(), viewToWorldCoord.z(), 1.0f);
		viewToWorldCoord4 = m_ViewToWorldMtx * viewToWorldCoord4;
		viewToWorldCoord = QVector3D(viewToWorldCoord4.x(), viewToWorldCoord4.y(), viewToWorldCoord4.z());
	}

	bool IsOutside(const QVector3D& location) const
	{
		float x = location.x();
		float y = location.y();
		float z = location.z();

		//if ((x >= m_maxX) || (y >= m_maxY) || (z > m_maxZ) || (x <= m_minX) || (y <= m_minY) || (z <= m_minZ))
		//	return true;

		if (x >= m_maxX)
			return true;
		else if (y >= m_maxY)
			return true;
		else if (z > m_maxZ)
			return true;
		else if (x <= m_minX)
			return true;
		else if (y <= m_minY)
			return true;
		else if (z <= m_minZ)
			return true;
		else
			return false;
	}

	Sphere m_sphere;
	float m_deltaS;
	QMatrix4x4 m_worldToViewMtx;
	QMatrix4x4 m_ViewToWorldMtx;
	DensFunc m_densityFunc;
	ColorFunc m_colorFunc;

	float m_minX;
	float m_minY;
	float m_minZ;

	float m_maxX;
	float m_maxY;
	float m_maxZ;

	int m_saturationThreshold;
};

#endif