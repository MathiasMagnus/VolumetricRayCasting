// SYCL include
#include <CL/sycl.hpp>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <array>

#include "MainWindow.h"


template <typename D, typename C>
auto make_RayCaster(D&& d, C&& c) { return Raycaster<D, C>(std::forward<D>(d), std::forward<C>(c)); }

int main(int argc, char *argv[])
{
	// Sample params
	const std::size_t plat_index = std::numeric_limits<std::size_t>::max();
	const std::size_t dev_index = std::numeric_limits<std::size_t>::max();
	const auto dev_type = cl::sycl::info::device_type::gpu;
	const std::size_t length = 4096u;

	try
	{

		// Platform selection
		auto plats = cl::sycl::platform::get_platforms();

		if (plats.empty()) throw std::runtime_error{ "No OpenCL platform found." };

		std::cout << "Found platforms:" << std::endl;
		for (const auto plat : plats) std::cout << "\t" << plat.get_info<cl::sycl::info::platform::vendor>() << std::endl;

		auto plat = plats.at(plat_index == std::numeric_limits<std::size_t>::max() ? 0 : plat_index);

		std::cout << "\n" << "Selected platform: " << plat.get_info<cl::sycl::info::platform::vendor>() << std::endl;

		// Device selection
		auto devs = plat.get_devices(dev_type);

		if (devs.empty()) throw std::runtime_error{ "No OpenCL device of specified type found on selected platform." };

		auto dev = devs.at(dev_index == std::numeric_limits<std::size_t>::max() ? 0 : dev_index);

		std::cout << "Selected device: " << dev.get_info<cl::sycl::info::device::name>() << "\n" << std::endl;


		// example lambda functions that could be given by the user
		// density function(spherical harminics) inside the extent
		auto densFunction = [](const float& r, const float& theta, const float& phi)
		{
			auto val = 1.0 / 2.0 * sqrt(3 / M_PI) * cos(theta); // Y(l = 1, m = 0)
																//out << val << "\t" << r << "\t" << theta * 180 / PI << "\t" << phi * 180 / PI << std::endl;
			if (abs(2 * abs(val) - r) < 0.01) // thickness of shell
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
		
		// extent definition
		std::array<std::array<float, 2>, 3 > extent = { { { -1.0, 1.0 },{ -1.0, 1.0 },{ -1.0, 1.0 } } };

		auto rayCaster = make_RayCaster(densFunction, colorFunction);
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
