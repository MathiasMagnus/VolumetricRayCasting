#include "SceneData.h"

SceneData::SceneData(const QVector3D& cameraPosition, const QVector3D& viewedPosisiton,
	const float fieldOfView, const std::array<std::array<float, 2>, 3 > &extent,
	std::function<float(float, float, float)> &function)
{
	cameraPositionData = cameraPosition;
	viewedPosisitonData = viewedPosisiton;
	fieldOfViewData = fieldOfView;
	extentData = extent;
	functionData = function;
}


SceneData::~SceneData()
{
}
