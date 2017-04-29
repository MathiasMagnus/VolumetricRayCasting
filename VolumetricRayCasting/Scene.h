//Scene.h
//Travis Gorkin
//Copyright (c)2009

#ifndef INCLUDED_SCENE_H
#define INCLUDED_SCENE_H

#include "Sphere.h"
#include "Camera.h"
#include "Light.h"
#include <vector>
#include <string>
#include <iostream>
#include <QMatrix4x4>
#include <array>
#include <functional>

//Height and Width of output image in pixels
#define WIDTH 400
#define HEIGHT 300

//Epsilon used for floating point error
#ifndef EPSILON
#define EPSILON 0.001
#endif

class Scene
{
public:
	Scene();
	void Init(const QVector3D& cameraPosition, const QVector3D& viewedPosition, const float fieldOfView);
	void TransformToViewCoordinates(QVector3D & worldToViewCoord) const;
	void TransformToWorldCoordinates(QVector3D & viewToWorldCoord) const;
	void Raycast();
	void WriteImageFile(std::string filename);
	void CreateTransformMatrices();
	void SetLimits(const std::array<std::array<float, 2>, 3 > &extent);
	bool IsOutside(const QVector3D& location) const;
	const std::vector<unsigned char>  GetRGBData() const;
	QVector3D GetCameraPos() const;
	QVector3D GetCameraViewDir() const;
	QVector3D GetCameraViewedPos() const;
	void SetCameraPos(const QVector3D &cameraPos);

	float m_maxAccumulatedDensity; 
	Sphere m_sphere;
	Camera m_camera;
	float m_deltaS;
	
	std::vector<QVector4D> m_pixelColors;
	QVector4D m_backgroundColor;
	Light m_light;
	QMatrix4x4 m_worldToViewMtx;
	QMatrix4x4 m_ViewToWorldMtx;
	std::function<float(float, float, float)> m_densityFunc;
	float m_minX;
	float m_minY;
	float m_minZ;

	float m_maxX;
	float m_maxY;
	float m_maxZ;

	QVector4D Raymarch(const QVector3D& rayDirection, float startT, float endT);
	QVector4D ColoringFunction(const int density);

};

#endif