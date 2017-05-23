#include <fstream>
#include <iostream>
#include <array>

#include "MainWindow.h"

template <typename D, typename C>
auto make_RayCaster(D&& d, C&& c) { return Raycaster<D, C>(std::forward<D>(d), std::forward<C>(c)); }

int main(int argc, char *argv[])
{
	try
	{
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
				return QColor(0, 0, 1); // blue
			}
			else if (density < 0)
			{
				return QColor(1, 1, 0); // yellow
			}
			else
				return  QColor(0, 0, 0); // black
		};
		QApplication a(argc, argv);
		
		// extent definition
		std::array<std::array<float, 2>, 3 > extent = { { { -1.0, 1.0 },{ -1.0, 1.0 },{ -1.0, 1.0 } } };

		auto rayCaster = make_RayCaster(densFunction, colorFunction);
		rayCaster.SetLimits(extent);

		MainWindow w;
		// camera position, viewed posisiton, view up, field of view
		Camera camera(QVector3D(0.0, 0.0, 2.2), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0), 120.0);
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
