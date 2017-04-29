#include <QGenericMatrix>
#include "Scene.h"
#include  <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Scene::Scene()
{
	m_backgroundColor = QVector4D(0.0f, 0.0f, 0.0f, 0.0f); 
	m_light = Light(QVector3D(1.0f, 1.0f, 1.0f), QVector3D(0.0f, 4.0f, -1.0f));
	m_maxAccumulatedDensity = 0.0; 
}

void Scene::SetLimits(const std::array<std::array<float, 2>, 3 > &extent)
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

void Scene::Init(const QVector3D& cameraPosition, const QVector3D& viewedPosition, const float fieldOfView)
{
	m_camera.m_position = cameraPosition;
	m_camera.m_viewedPosition = viewedPosition;
	m_camera.m_viewDir = QVector3D(m_camera.m_viewedPosition - m_camera.m_position);
	m_camera.m_viewDir.normalize();
	m_camera.m_viewUp = QVector3D(0.0, 1.0, 0.0);
	m_camera.m_fieldOfView = fieldOfView; // in degrees
}

// #Q: should the m_viewUp also be transformed when I move t
void Scene::CreateTransformMatrices()
{
	m_worldToViewMtx.setToIdentity();
	m_ViewToWorldMtx.setToIdentity();
	m_worldToViewMtx.lookAt(m_camera.m_position, m_camera.m_viewedPosition, m_camera.m_viewUp);
	m_ViewToWorldMtx = m_worldToViewMtx.inverted(); // is this always invertible? if not, than wut?
}

void Scene::TransformToViewCoordinates(QVector3D & worldToViewCoord) const
{
	QVector4D  worldToViewCoord4(worldToViewCoord.x(), worldToViewCoord.y(), worldToViewCoord.z(), 1.0f);
	worldToViewCoord4 = m_worldToViewMtx * worldToViewCoord4;
	worldToViewCoord = QVector3D(worldToViewCoord4.x(), worldToViewCoord4.y(), worldToViewCoord4.z());
}

void Scene::TransformToWorldCoordinates(QVector3D & viewToWorldCoord) const
{
	QVector4D  viewToWorldCoord4(viewToWorldCoord.x(), viewToWorldCoord.y(), viewToWorldCoord.z(), 1.0f);
	viewToWorldCoord4 = m_ViewToWorldMtx * viewToWorldCoord4;
	viewToWorldCoord = QVector3D(viewToWorldCoord4.x(), viewToWorldCoord4.y(), viewToWorldCoord4.z());
}

//Initiate the volume rendering of the scene by raycasting/raymarching for each pixel
void Scene::Raycast()
{
	m_pixelColors.clear();
	m_pixelColors.reserve(WIDTH * HEIGHT);
	m_maxAccumulatedDensity = 0;
	CreateTransformMatrices();

	float aspectRatio = (float)WIDTH / (float)HEIGHT;
	float x, y, z;
	
	z = -1.0f;

	float scaleFOV = tan(m_camera.m_fieldOfView / 2 * M_PI / 180);

	for(unsigned int i=0; i < HEIGHT; i++)
	{	
		y = (1 - 2 * (i + 0.5) / (float)HEIGHT) * scaleFOV;
		for(unsigned int j=0; j < WIDTH; j++)
		{	
			x = (2 * (j + 0.5) / (float)WIDTH - 1) * aspectRatio * scaleFOV;
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
			QVector3D transformedCamRayDir = rayVec - m_camera.GetPosition();
			transformedCamRayDir.normalize();
			bool bIntersected = m_sphere.GetIntersections(m_camera.GetPosition(), transformedCamRayDir, t0, t1);
			
			if (bIntersected && t0 > 0.0 && t1 > 0.0)
			{
				m_pixelColors.push_back(Raymarch(transformedCamRayDir, t0, t1));
			}
			// if we are inside the spehere, we trace from the the ray's original position
			else if (bIntersected && t1 > 0.0)   
			{
				m_pixelColors.push_back(Raymarch(transformedCamRayDir, 0.0, t1));
			}
			else
			{
				m_pixelColors.push_back(m_backgroundColor);
			}
		}
	}
}

//std::ofstream out("C:/Users/balaz/Desktop/degree_atan2.txt");
//Raymarch from startT to endT, accumulating density
QVector4D Scene::Raymarch(const QVector3D& rayDirection, float startT, float endT)
{
	QVector4D color4D(0.0f, 0.0f, 0.0f, 0.0f);
	QVector3D location(0.0f, 0.0f, 0.0f);

	location = m_camera.GetPosition() + startT*rayDirection;

	float current_t = startT;
	// accumulated legyen 4es vektor, elso 3 szin, 4-ik alpha csatorna(telitettseg)
	//float accumulatedDensity = 0.0;
	QVector4D accumulatedDensity;

	while(current_t < endT)
	{
		location = location + m_deltaS*rayDirection;
		current_t += m_deltaS;
		if (!IsOutside(location)) 
		{
			// atvaltani xyz-t rthetaphire/
			// Convert to spherical coordinated
			
			float r = sqrt(location.x()*location.x() + location.y()*location.y() + location.z()*location.z());
			//if (r < 0.9 || r > 1.0)
				//continue;
			float theta = acos(location.z() / r) ; //* 180 / M_PI; // convert to degrees?
			float phi = atan2(location.y(), location.x()) ; //* 180 / M_PI;
	//		out << r << "\t" << theta << "\t" << phi << std::endl;
			/*
			-a density fv. visszad egy akarmit? (pl. ha komplex, akkor hogy?)
			-a szinezo irom bele, hogyha pozitiv, akkor a piros erteket irom bele, ha negativ, akkor a masikba
			-a m_densityFunc eredmenyet rogton beadni a myColoringFunctionbe
			-a myColorinf funtion eredemenyet csak akkor irom be az accumualted density eredmenyebe,
			ha az alfa csatorna nincs szaturalodva
			*/
			QVector4D color = ColoringFunction(m_densityFunc(r, theta, phi));
			if (accumulatedDensity.w() < 1.0)
				accumulatedDensity += color;

			//accumulatedDensity += m_densityFunc(r, theta, phi);
			//// colorFunction(accumulatedDensit4es),
			//{
			//	return 4esVektor; // csak akkor irom be az accumulated densitybe, ha nem szaturalodott az ertek
			//}
			
			// end conversion
			//accumulatedDensity += m_densityFunc(location.x(), location.y(), location.z());
		}
	}
	
	// this is now handled by the saturation parameter?
	//if (accumulatedDensity > m_maxAccumulatedDensity) // needed for normalization
	//	m_maxAccumulatedDensity = accumulatedDensity;
	
	// az accumulated desnity egy r,g,b vektor legyen(4es vektor, 3 szin,4ik az alfa csatorna = telitettseg)
	//color4D = m_backgroundColor + accumulatedDensity * m_light.GetColor();
	color4D = m_backgroundColor + accumulatedDensity ;
	return color4D;
}

QVector4D Scene::ColoringFunction(const int density)
{
	if (density > 0)
	{ 
		return QVector4D(0.0, 0.0, 1.0, 0.1); // blue
	}
	else if (density < 0)
	{	
		return QVector4D(1.0, 1.0, 0.0, 0.1); // yellow
	}
	else
		return  QVector4D(0.0, 0.0, 0.0, 0.0); // black
}

bool Scene::IsOutside(const QVector3D& location) const
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

//Scene::WriteImageFile
//Write array of pixel colors to bmp file
//std::ofstream outOfs("C:/Users/balaz/Desktop/VR_2/VolumeRenderer/rgb1.8_.txt");
void Scene::WriteImageFile(std::string filename)
{
	std::ostringstream out;
	out.clear();
	unsigned char r, g, b;
	for(unsigned int i = 0; i < m_pixelColors.size(); i++)
	{
		r = (unsigned char)(std::min(m_pixelColors[i].x() / m_maxAccumulatedDensity * 255., 255.));
		g = (unsigned char)(std::min(m_pixelColors[i].y() / m_maxAccumulatedDensity * 255., 255.));
		b = (unsigned char)(std::min(m_pixelColors[i].z() / m_maxAccumulatedDensity * 255., 255.));
		
		out << r << g << b;
		//outOfs << m_pixelColors[i].x() << " " << m_pixelColors[i].y() << " " << m_pixelColors[i].z() << std::endl;
	}
	out.flush();

	//SOIL_save_image(filename.c_str(), SOIL_SAVE_TYPE_BMP, WIDTH, HEIGHT, 3, (const unsigned char *)out.str().c_str());	
}

const std::vector<unsigned char> Scene::GetRGBData() const
{
	std::vector<unsigned char> rgbData;
	rgbData.reserve(m_pixelColors.size());
	unsigned char r, g, b;
	for (unsigned int i = 0; i < m_pixelColors.size(); i++)
	{
		//if (m_pixelColors[i].w() > 0.0)
		//	std::cout << "mia";
		r = (unsigned char)(std::min(m_pixelColors[i].x()*20.0, 255.));
		g = (unsigned char)(std::min(m_pixelColors[i].y()*20.0, 255.));
		b = (unsigned char)(std::min(m_pixelColors[i].z()*20.0, 255.));

		rgbData.push_back(r);
		rgbData.push_back(g);
		rgbData.push_back(b);
	}

	return rgbData;
}

QVector3D Scene::GetCameraPos() const
{
	return m_camera.GetPosition();
}

QVector3D Scene::GetCameraViewDir() const
{
	return m_camera.GetViewDir();
}

QVector3D Scene::GetCameraViewedPos() const
{
	return m_camera.GetViewedPos();
}

void Scene::SetCameraPos(const QVector3D &cameraPos)
{
	m_camera.SetPosition(cameraPos);
}
