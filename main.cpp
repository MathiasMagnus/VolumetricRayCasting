// SYCL include
#include <CL/sycl.hpp>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <array>
#include <QCommandLineParser>

#include "MainWindow.h"


template <typename D, typename C>
auto make_RayCaster(D&& d, C&& c, size_t plat_id, cl::sycl::info::device_type device_type) { return Raycaster<D, C>(std::forward<D>(d), std::forward<C>(c), plat_id, device_type); }

int main(int argc, char *argv[])
{
	// Sample params
	const std::size_t plat_index = std::numeric_limits<std::size_t>::max();
	const std::size_t dev_index = std::numeric_limits<std::size_t>::max();
	const auto dev_type = cl::sycl::info::device_type::gpu;
	const std::size_t length = 4096u;

	try
	{
		// example lambda functions that could be given by the user
		// density function(spherical harminics) inside the extent
		auto densFunction = [](const float& r, const float& theta, const float& phi)
		{
#ifdef __SYCL_DEVICE_ONLY__
            float sqrt3fpi = cl::sycl::sqrt(3.0f / M_PI);
            float val = 1.0f / 2.0f * sqrt3fpi * cl::sycl::cos(theta); // Y(l = 1, m = 0)
            float result = cl::sycl::fabs(2 * cl::sycl::fabs(val) - r);
#else
            float sqrt3fpi = 1.0f;
            float val = 1.0f;
            float result = 1.0f;
#endif
            if (result < 0.01f)	// thickness of shell 
				return val < 0 ? -1 : 1;
			else
				return 0;
		};

		// color according to the incoming density
		auto colorFunction = [](const int density)
		{
			if (density > 0)
			{
				return cl::sycl::uchar4(0, 0, 1, 0); // blue
			}
			else if (density < 0)
			{
				return cl::sycl::uchar4(1, 1, 0, 0); // yellow
			}
			else
				return  cl::sycl::uchar4(0, 0, 0, 0); // black
		};
		QApplication a(argc, argv);

		QCommandLineParser parser;
		parser.setApplicationDescription("Sample application demonstrating OpenCL-OpenGL interop");
		parser.addHelpOption();
		parser.addVersionOption();
		parser.addOptions({
			{ { "d", "device" }, "Device type to use", "[cpu|gpu|acc]", "cpu" },
			{ { "p", "platformId" }, "The index of the platform to use", "unsigned integral", "0" },
			{ { "x", "particles" }, "Number of particles", "unsigned integral", "8192" }
			});

		parser.process(a);

		cl::sycl::info::device_type dev_type;
		std::size_t plat_id, count;
		if (!parser.value("device").isEmpty())
		{
			if (parser.value("device") == "cpu")
				dev_type = cl::sycl::info::device_type::cpu;
			else if (parser.value("device") == "gpu")
				dev_type = cl::sycl::info::device_type::gpu;
			else if (parser.value("device") == "host")
				dev_type = cl::sycl::info::device_type::host;
			else if (parser.value("device") == "acc")
				dev_type = cl::sycl::info::device_type::accelerator;
			else
			{
				qFatal("NBody: Invalid device type: valid values are [cpu|gpu|acc]. Using CL_DEVICE_TYPE_DEFAULT instead.");
			}
		}
		else dev_type = cl::sycl::info::device_type::automatic;
		if (!parser.value("platformId").isEmpty())
		{
			plat_id = parser.value("platformId").toULong();
		}
		else plat_id = 0;
		if (!parser.value("particles").isEmpty())
		{
			count = parser.value("particles").toULong();
		}
		
		// extent definition
		std::array<std::array<float, 2>, 3 > extent = { { { -1.0, 1.0 },{ -1.0, 1.0 },{ -1.0, 1.0 } } };

		auto rayCaster = make_RayCaster(densFunction, colorFunction, plat_id, dev_type);
		rayCaster.SetLimits(extent);

		MainWindow w;
		// camera position, viewed posisiton, view up, field of view
		Camera camera(glm::vec3(0.0, 0.0, 2.2), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), 120.0);
		w.SetRayCaster(rayCaster, camera);
		w.Raycast();
		w.show();
		return a.exec();

	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << "\nFatal error\n" << std::endl;
	}

	std::cout << "Press any key to continue...\n";
	getchar();
	return 0;
	
}
