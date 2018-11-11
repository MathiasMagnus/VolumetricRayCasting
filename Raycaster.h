#ifndef INCLUDED_SCENE_H
#define INCLUDED_SCENE_H

#include <functional>
//#define _USE_MATH_DEFINES
//#include <math.h>
# define M_PI           3.14159265358979323846  /* pi */
#include <cmath>
#include "Sphere.h"
#include "Camera.h"
#include <CL/sycl.hpp>
#include <iostream>
#include <string>
// GLM includes
//#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include "random"
template <typename DensFunc, typename ColorFunc>
class Raycaster
{
public:
	Raycaster(DensFunc densFunc, ColorFunc colorFunc, size_t plat_id, cl::sycl::info::device_type device_type) : m_densityFunc(densFunc), m_colorFunc(colorFunc), 
		m_plat_id(plat_id), m_device_type(device_type)
	{
		m_saturationThreshold = 0;
	}

	void operator()(QImage& image, int imageWidth, int imageHeight, const  Camera& camera)
	{
		//SimpleMatrixMult();
		RaycastGPU(image, imageWidth, imageHeight, camera);
	}

	//Initiate the volume rendering of the scene by raycasting/raymarching for each pixel
	//void Raycast(QImage& inImage, int imageWidth, int imageHeight, const Camera& camera)
	//{
	//	//std::ofstream ofs("bitNr.txt");
	//	auto bytePointer = inImage.bits();
	//	int byteCount = 0;

	//	CreateTransformMatrices(camera);

	//	float aspectRatio = (float)imageWidth / (float)imageHeight;
	//	float x, y, z;

	//	z = -1.0f;

	//	float scaleFOV = tan(camera.m_fieldOfView / 2 * M_PI / 180);

	//	for (unsigned int i = 0; i < imageHeight; i++)
	//	{
	//		y = (1 - 2 * (i + 0.5) / (float)imageHeight) * scaleFOV;
	//		for (unsigned int j = 0; j < imageWidth; j++)
	//		{
	//			x = (2 * (j + 0.5) / (float)imageWidth - 1) * aspectRatio * scaleFOV;
	//			QVector3D rayVec(x, y, z);

	//			float t0 = -1E+36;
	//			float t1 = -1E+36;

	//			TransformToWorldCoordinates(rayVec);
	//			QVector3D transformedCamRayDir = rayVec - camera.GetPosition();
	//			transformedCamRayDir.normalize();
	//			bool bIntersected = m_sphere.GetIntersections(camera.GetPosition(), transformedCamRayDir, t0, t1);

	//			QColor pixelColor;
	//			if (bIntersected && t0 > 0.0 && t1 > 0.0)
	//			{
	//				pixelColor = Raymarch(camera.GetPosition(), transformedCamRayDir, t0, t1);
	//			}
	//			// if we are inside the spehere, we trace from the the ray's original position
	//			else if (bIntersected && t1 > 0.0)
	//			{
	//				pixelColor = Raymarch(camera.GetPosition(), transformedCamRayDir, 0.0, t1);
	//			}
	//			else
	//			{
	//				pixelColor = QColor(0, 0, 0);
	//			}

	//			// seting rgb value for every pixel
	//			bytePointer[byteCount] = pixelColor.red();
	//			byteCount++;
	//			bytePointer[byteCount] = pixelColor.green();
	//			byteCount++;
	//			bytePointer[byteCount] = pixelColor.blue();
	//			byteCount++;
	//			//ofs << byteCount << std::endl;
	//		}
	//	}
	//}

	//Initiate the volume rendering of the scene by raycasting/raymarching for each pixel
	// TD: param by value(inside kernel), use non member variables

	void SimpleMatrixMult() {
		const std::size_t length = 4096u;
		cl::sycl::queue queue = getQueue();
		cl::sycl::buffer<float> buf{ cl::sycl::range<1>{length} };

		/*{
			auto access = buf.get_access<cl::sycl::access::mode::write>();

			std::fill_n(access.get_pointer(), access.get_count(), 1.f);
		}
*/
		queue.submit([&](cl::sycl::handler& cgh)
		{
			auto v = buf.get_access<cl::sycl::access::mode::read_write>(cgh);
			cgh.parallel_for<class SYCL_Test>(v.get_range(), [=](cl::sycl::item<1> i)
			{
				//glm::mat4 mtx(2.0, 0.0, 0.0, 0.0,
				//	0.0, 2.0, 0.0, 0.0,
				//	0.0, 0.0, 2.0, 0.0,
				//	0.0, 0.0, 0.0, 1.0);

				// glm code example
				//auto vec = glm::vec3(0.2, 0.6, 1.0);
				//float angle = 90; // since glm version 0.9.6, rotations are in radians, not in degrees
				//				  // see https://github.com/g-truc/glm/issues/570
				//cout << vec << endl;
				//auto rotatedVec = glm::rotate(vec, angle, glm::vec3(0.0f, 1.0f, 0.0f));
				//cout << rotatedVec << endl;
				// END glm code example

				//QVector3D x2 = mtx * x1;
				glm::vec4 v1{ 1.f, 1.f, 1.f, 1.f };
				/*glm::mat4 mtx(1.0f);
				glm::vec4 v2 = mtx * v1;*/
				auto v3 = glm::normalize(v1);
				auto res = glm::dot(v1, v3);

				v[i] = 0;

				std::minstd_rand prng{ (unsigned int)i.get_linear_id()};
				std::uniform_int<int> dist{ 0, 1 };
				if (dist(prng))
					v[i] += 1;
				else
					v[i] += 4;			
			});
		});

		auto vec = buf.get_access <cl::sycl::access::mode::read>();
		std::ofstream ofs("simple.txt");
		for (size_t i = 0; i < length; i++) {
			ofs << vec[i] << " ";
		}

		ofs.close();

	}

	cl::sycl::queue getQueue() {

		// Platform selection
		auto plats = cl::sycl::platform::get_platforms();

		if (plats.empty()) throw std::runtime_error{ "No OpenCL platform found." };

		//std::cout << "Found platforms:" << std::endl;
		//for (const auto plat : plats) std::cout << "\t" << plat.get_info<cl::sycl::info::platform::vendor>() << std::endl;

		auto plat = plats.at(m_plat_id);

		//std::cout << "\n" << "Selected platform: " << plat.get_info<cl::sycl::info::platform::vendor>() << std::endl;

		// Device selection
		auto devs = plat.get_devices(m_device_type);

		if (devs.empty()) throw std::runtime_error{ "No OpenCL device of specified type found on selected platform." };

		auto dev = devs.at(0);

		//std::cout << "Selected device: ";// << dev.get_info<cl::sycl::info::device::name>() << "\n" << std::endl;

		return cl::sycl::queue(dev);

	}

	void RaycastGPU(QImage& inImage, const size_t imageWidth, const size_t imageHeight, const Camera& camera)
	{
		try {
			// Start raymarch lambda
			auto m_raymarch = [](const glm::vec3& camPos, const glm::vec3& rayDirection, const float startT, const float endT, const float deltaS, const Extent extent, const int saturationThreshold,
				DensFunc densityFunc, ColorFunc colorFunc)
			{
				cl::sycl::uchar4 finalColor(0, 0, 0, 0);
				glm::vec3 location(0.0f, 0.0f, 0.0f);

				location = camPos + startT * rayDirection;

				float current_t = startT;

				while (current_t < endT)
				{
					location = location + deltaS * rayDirection;
					current_t += deltaS;

					// check if it is inside
					//if (!IsOutside(location))
					if ((location.x < extent.m_maxX) && (location.y < extent.m_maxY) && (location.z < extent.m_maxZ) && (location.x > extent.m_minX) && (location.y > extent.m_minY) && (location.z > extent.m_minZ))
					{
						// Convert to spherical coordinated
						float r = sqrt(location.x*location.x + location.y*location.y + location.z*location.z);
						float theta = acos(location.z / r); //* 180 / M_PI; // convert to degrees?
						float phi = atan2(location.y, location.x); //* 180 / M_PI;

						cl::sycl::uchar4 color = colorFunc(1);
						//QColor color = colorFunc(densityFunc(r, theta, phi));


						finalColor += color;

						/*finalColor.setRed(finalColor.red() + color.red());
						finalColor.setGreen(finalColor.green() + color.green());
						finalColor.setBlue(finalColor.blue() + color.blue());*/
					}

					// stop the ray, when color reaches the saturation.
					if (finalColor.r() > saturationThreshold || finalColor.g() > saturationThreshold
						|| finalColor.b() > saturationThreshold)
						break;
				}

				// normalizer according to the highest rgb value
				auto normalizer = std::max((uchar)1, std::max(std::max(finalColor.r(), finalColor.g()), finalColor.b()));
				finalColor /= normalizer;
				finalColor *= 255;
				/*finalColor.r() = finalColor.r() / normalizer * 255;
				finalColor.g() = finalColor.g() / normalizer * 255;
				finalColor.b() = finalColor.b() / normalizer * 255;*/


				//cl::sycl::uchar4 colorSycl(finalColor.red(), finalColor.green(), finalColor.blue(), finalColor.alpha());
				return finalColor;
			};
			// END raymarch lambda

			CreateTransformMatrices(camera);

			float aspectRatio = (float)imageWidth / (float)imageHeight;
			//float x, y, z;

			//z = -1.0f;

			float scaleFOV = tan(camera.m_fieldOfView / 2 * M_PI / 180);

			// START GPU
			auto myQueue = getQueue();
			constexpr auto ss = sizeof(cl::sycl::vec < unsigned char, 3 >);
			cl::sycl::buffer < cl::sycl::uchar4, 2> resultBuff{ reinterpret_cast<cl::sycl::uchar4*>(inImage.bits()), cl::sycl::range<2> {imageHeight, imageWidth} };
			//cl::sycl::buffer < cl::sycl::vec < unsigned char, 4 >, 2> resultBuff{ reinterpret_cast<cl::sycl::vec < unsigned char, 4 >*>(inImage.bits()), cl::sycl::range<2> {imageHeight, imageWidth} };
			myQueue.submit([&](cl::sycl::handler& cgh) {
				auto imageData = resultBuff.get_access <cl::sycl::access::mode::write>(cgh);
				cgh.parallel_for<class raycast>(cl::sycl::range<2> {imageHeight, imageWidth}, [=,
					ViewToWorldMtx = m_ViewToWorldMtx,
					camPos = camera.GetPosition(), raymarch = m_raymarch, deltaS = m_deltaS, extent = m_extent, densityFunc = m_densityFunc, colorFunc = m_colorFunc,
					saturationThreshold = m_saturationThreshold,
					sphereCenter = m_sphere.m_center, sphereRadius2 = m_sphere.m_radius2
				](cl::sycl::id<2> index) {

					glm::vec4 rayVec((2 * (index[1] + 0.5f) / (float)imageWidth - 1) * aspectRatio * scaleFOV,
						(1 - 2 * (index[0] + 0.5f) / (float)imageHeight) * scaleFOV,
						-1.0f, 1.0f); // ADDED 4th element 1.0f to support 4x4 matrix multiplication

					float t0 = -1E+36;
					float t1 = -1E+36;
					
					//glm::vec3 transformedCamRayDir = glm::vec3(ViewToWorldMtx * rayVec) - camPos;

					//transformedCamRayDir = glm::normalize(transformedCamRayDir);
					//bool bIntersected = sphere.GetIntersections(cam.GetPosition(), transformedCamRayDir, t0, t1);

					auto getIntersections_lambda = [&t0, &t1, index](const cl::sycl::float3 rayorig, const cl::sycl::float3 raydir, const cl::sycl::float3 sphereCenter, const float sphereRadius2) {
						cl::sycl::float3 l = sphereCenter - rayorig;
						float tca = cl::sycl::dot(l, raydir);
						float d2 = cl::sycl::dot(l, l) - tca * tca;

						bool isIntersected = true;
						//if ((int)sphereRadius2  ==  (int)d2)
						//if ((sphereRadius2 - d2) < 0.0001f)
						if (index.get(1) % 2 == 0)
							isIntersected = false;

						float thc =
#ifdef __SYCL_DEVICE_ONLY__
							cl::sycl::sqrt(sphereRadius2 - d2);
#else
							0.f;
#endif
						t0 = tca - thc;
						t1 = tca + thc;

						return isIntersected;

					};

					/*auto bIntersected = getIntersections_lambda(
						cl::sycl::float3(cp.x, cp.y, cp.z),
						cl::sycl::float3(tcrd.x, tcrd.y, tcrd.z),
						cl::sycl::float3(sc.x, sc.y, sc.z),
						sphereRadius2);*/

					/*glm::vec3 l = sphereCenter - camPos;
					float tca = glm::dot(l, transformedCamRayDir);
					float d2 = glm::dot(l, l) - tca * tca;*/

					cl::sycl::uchar4 pixelColor;
					//auto camPosfloat3 = cl::sycl::float3{ rayVec.x, rayVec.y, rayVec.z };
					//if (index.get(1) % 2 == 0) {
					//if ((int(glm::length(camPos))) % 2 == 0) {
					auto length = (int)glm::dot(rayVec, rayVec);
					//auto length = (int)glm::dot(transformedCamRayDir, transformedCamRayDir) + 1;
					//auto length = (int)glm::dot(transformedCamRayDir, transformedCamRayDir) + 1;
					if (length % 2 == 0) {
					//if (index.get(1) % 2 == 0) {
					//if ((int(glm::length(camPos))) % 2 == 0) {
					//if ((int(cl::sycl::length(camPosfloat3))) % 2 == 0) {

						pixelColor = cl::sycl::uchar4(255, 0, 0, 255);
					}
					else if (index.get(1) % 3 == 0) {
						pixelColor = cl::sycl::uchar4(0, 255, 0, 255);
					}
					else if (index.get(1) % 5 == 0) {
						pixelColor = cl::sycl::uchar4(0, 0, 255, 255);
					}
					else if (index.get(1) % 7 == 0) {
						pixelColor = cl::sycl::uchar4(234, 55, 54, 255);
					}
					else if (index.get(1) % 11 == 0) {
						pixelColor = cl::sycl::uchar4(0, 255, 234, 255);
					}
					else {
						pixelColor = cl::sycl::uchar4(0, 0, 0, 255);

					}
					//bool bIntersected = true;
					////if ((int(glm::length(camPos))) % 2 == 0)
					////if ((sphereRadius2 - d2) < 0.0001f)
					//if (index.get(1) % 2 == 0)
					//	bIntersected = false;

//					float thc =
//#ifdef __SYCL_DEVICE_ONLY__
//						cl::sycl::sqrt(sphereRadius2 - d2);
//#else
//						0.f;
//#endif
//					t0 = tca - thc;
//					t1 = tca + thc;
//
//					cl::sycl::uchar4 pixelColor;
//					if (bIntersected)
//					//if (bIntersected && t0 > 0.0 && t1 > 0.0)
//					{
//						pixelColor = cl::sycl::uchar4(255, 0, 0, 255);
//						/*pixelColor = raymarch(cam.GetPosition(), transformedCamRayDir, t0, t1, deltaS, extent, saturationThreshold,
//							const_cast<DensFunc>densityFunc, const_cast<ColorFunc>(colorFunc));*/
//					}
//					// if we are inside the spehere, we trace from the the ray's original position
//					else if (bIntersected && t1 > 0.0)
//					{
//						//pixelColor = raymarch(cam.GetPosition(), transformedCamRayDir, 0.0, t1, deltaS, extent, saturationThreshold, const_cast<DensFunc>(densityFunc), const_cast<ColorFunc>(colorFunc));
//						pixelColor = cl::sycl::uchar4(0, 255, 0, 255);
//					}
//					else
//					{
//						pixelColor = cl::sycl::uchar4(0, 0, 255, 255);
//					}

					// seting rgb value for every pixel
					imageData[index] = pixelColor;	
				});

			});

			// copy back is done automatically
			/*auto imageData = resultBuff.get_access <cl::sycl::access::mode::read>();
			std::copy(imageData.get_pointer(), imageData.get_pointer() + imageData.get_count(), reinterpret_cast<cl::sycl::vec < unsigned char, 4 >*>(inImage.bits()));*/
		}
		catch (cl::sycl::exception e)
		{
			std::ofstream ofs2("after1.txt");
			ofs2 << "e.what(): " << e.what() << std::endl;
			ofs2 << "e.get_cl_code(): " << e.get_cl_code() << std::endl;
			ofs2 << "e.get_cl_error_message(): " << e.get_cl_error_message() << std::endl;
			//ofs2 << e.get_context() << std::endl;
			ofs2 << "(int)e.get_cpp_error_code(): " << (int)e.get_cpp_error_code() << std::endl;
			ofs2 << "e.get_description(): " << e.get_description() << std::endl;
			ofs2 << "e.get_file_name(): " << e.get_file_name() << std::endl;
			ofs2 << "e.get_line_number(): " << e.get_line_number() << std::endl;
			ofs2.close();
			inImage.save("after1.png");
			std::cerr << e.what() << std::endl;
			//std::cin.get();
			std::exit(e.get_cl_code());
		}
		catch (std::exception e)
		{
			std::ofstream ofs2("after2.txt");
			ofs2 << "aaaaa";
			ofs2.close();
			inImage.save("after2.png");
			std::cerr << e.what() << std::endl;
			//std::cin.get();
			std::exit(EXIT_FAILURE);
		}
	}

	// END GPU

//std::ofstream out("C:/Users/balaz/Desktop/degree_atan2.txt");
//Raymarch from startT to endT, accumulating density
// return uchar4 vec sycl uchar4 instead of Qcolor
	cl::sycl::uchar4 Raymarch(const glm::vec3& camPos, const glm::vec3& rayDirection, float startT, float endT)
	{
		QColor finalColor(0, 0, 0, 0);
		glm::vec3 location(0.0f, 0.0f, 0.0f);

		location = camPos + startT * rayDirection;

		float current_t = startT;

		while (current_t < endT)
		{
			location = location + m_deltaS * rayDirection;
			current_t += m_deltaS;
			if (!IsOutside(location))
			{
				// Convert to spherical coordinated
				float r = sqrt(location.x*location.x + location.y*location.y + location.z*location.z);
				float theta = acos(location.z / r); //* 180 / M_PI; // convert to degrees?
				float phi = atan2(location.y, location.x); //* 180 / M_PI;

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


		cl::sycl::uchar4 colorSycl(finalColor.red(), finalColor.green(), finalColor.blue(), finalColor.alpha());
		return colorSycl;
	}

	void SetLimits(const std::array<std::array<float, 2>, 3 > &extent)
	{
		m_extent.m_minX = extent[0][0];
		m_extent.m_maxX = extent[0][1];

		m_extent.m_minY = extent[1][0];
		m_extent.m_maxY = extent[1][1];

		m_extent.m_minZ = extent[2][0];
		m_extent.m_maxZ = extent[2][1];

		if (m_extent.m_minX > m_extent.m_maxX || m_extent.m_minY > m_extent.m_maxY || m_extent.m_minZ > m_extent.m_maxZ)
			throw std::runtime_error("Error: higher limit should be bigger than the lower!");

		// find center of the encapsulating sphere
		m_sphere.SetCenter(glm::vec3((m_extent.m_minX + m_extent.m_maxX) / 2, (m_extent.m_minY + m_extent.m_maxY) / 2, (m_extent.m_minZ + m_extent.m_maxZ) / 2));
		float maxRadius = std::max({ (m_extent.m_maxX - m_extent.m_minX) , (m_extent.m_maxY - m_extent.m_minY) , (m_extent.m_maxZ - m_extent.m_minZ) }) / 2;
		m_sphere.SetRadius(maxRadius*1.8);

		m_deltaS = std::min({ (m_extent.m_maxX - m_extent.m_minX) , (m_extent.m_maxY - m_extent.m_minY) , (m_extent.m_maxZ - m_extent.m_minZ) }) / 100; //  step size
	}

	void CreateTransformMatrices(const Camera& camera)
	{
		//m_worldToViewMtx.setToIdentity();
		//m_ViewToWorldMtx.setToIdentity();
		//m_worldToViewMtx.lookAt(camera.m_position, camera.m_viewedPosition, camera.m_viewUp);
		//m_ViewToWorldMtx = m_worldToViewMtx.inverted(); // is this always invertible? if not, than wut?


		m_worldToViewMtx = glm::lookAt(camera.m_position, camera.m_viewedPosition, camera.m_viewUp);
		m_ViewToWorldMtx = glm::inverse(m_worldToViewMtx);
	}

	/*void TransformToViewCoordinates(glm::vec3 & worldToViewCoord) const
	{
		QVector4D  worldToViewCoord4(worldToViewCoord.x, worldToViewCoord.y, worldToViewCoord.z(), 1.0f);
		worldToViewCoord4 = m_worldToViewMtx * worldToViewCoord4;
		worldToViewCoord = glm::vec3(worldToViewCoord4.x, worldToViewCoord4.y(), worldToViewCoord4.z());
	}

	void TransformToWorldCoordinates(glm::vec3 & viewToWorldCoord) const
	{
		QVector4D  viewToWorldCoord4(viewToWorldCoord.x, viewToWorldCoord.y(), viewToWorldCoord.z(), 1.0f);
		viewToWorldCoord4 = m_ViewToWorldMtx * viewToWorldCoord4;
		viewToWorldCoord = glm::vec3(viewToWorldCoord4.x, viewToWorldCoord4.y(), viewToWorldCoord4.z());
	}*/

	Sphere m_sphere;
	float m_deltaS;
	glm::mat4 m_worldToViewMtx;
	glm::mat4 m_ViewToWorldMtx;
	DensFunc m_densityFunc;
	ColorFunc m_colorFunc;
	size_t m_plat_id;
	cl::sycl::info::device_type m_device_type;


	/*float m_minX;
	float m_minY;
	float m_minZ;

	float m_maxX;
	float m_maxY;
	float m_maxZ*/;

	struct Extent
	{
		float m_minX;
		float m_minY;
		float m_minZ;

		float m_maxX;
		float m_maxY;
		float m_maxZ;
	};

	Extent m_extent;
	int m_saturationThreshold;
};

#endif