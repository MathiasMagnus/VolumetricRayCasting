#pragma once
#include <QVector3D>
#include <functional>
#include <array>
class SceneData
{
public:
	SceneData(const QVector3D& cameraPosition, const QVector3D& viewedPosisiton,
		const float fieldOfView, const std::array<std::array<float, 2>, 3 > &extent,
		std::function<float(float, float, float)> &function);
	
	QVector3D cameraPositionData;
	QVector3D viewedPosisitonData;
	float fieldOfViewData;
	std::array<std::array<float, 2>, 3 > extentData;
	std::function<float(float, float, float)> functionData;
	~SceneData();
};

