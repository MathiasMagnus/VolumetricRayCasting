#ifndef VolumetricRayCasting_H
#define VolumetricRayCasting_H

#include <QtWidgets/QMainWindow>
#include "ui_VolumetricRayCasting.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include "GraphicsScene.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	template <typename RC>
	void SetRayCaster(RC&& rc, const Camera& camera)
	{
		m_graphicsScene->SetRayCaster(rc, camera);
	}

	void Raycast();
	void SetCameraPos(const QVector3D& cameraPos);

private:
	Ui::VolumetricRayCastingClass ui;
	QGraphicsView * view;
	GraphicsScene * m_graphicsScene;
};

#endif // VolumetricRayCasting_H
