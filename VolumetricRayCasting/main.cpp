#include "MainWindow.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>


#include "Scene.h"
#include "SceneData.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <array>
#include <tuple>
#define PI 3.14159265


int main(int argc, char *argv[])
{
	try
	{
		QApplication a(argc, argv);
		
		std::function<float(float, float, float)> densFunction = [](const float& r, const float& theta, const float& phi)
		{	
			auto val = 1.0 / 2.0 * sqrt(3 / PI) * cos(theta);
			////out << val << "\t" << r << "\t" << theta * 180 / PI << "\t" << phi * 180 / PI << std::endl;
			if (abs(2*abs(val) - r) < 0.01) // thickness of shell
				return val < 0 ? -1 : 1;
			else
				return 0;
		};
		

		
		// TD: should be a template class, in the cpn constructor
		SceneData sceneData(
			QVector3D(0.0, 0.0, 2.2),								//	camera position
			QVector3D(0.0, 0.0, 0.0),								//	viewed posisiton
			120.0,													// field of view
			{ { { -1.0, 1.0 },{ -1.0, 1.0 },{ -1.0, 1.0 } } },		// extent
			densFunction);											// density function

		MainWindow w;
		w.SetScene(sceneData);
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
