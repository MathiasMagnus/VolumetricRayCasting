#pragma once

// Raycaster includes
#include "GraphicsScene.hpp"

// Qt includes
#include <QtWidgets/QMainWindow>
#include <QGraphicsView>

// Generated headers
#include "ui_VolumetricRayCasting.h"


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
	//Ui::VolumetricRayCastingClass ui;
	QGraphicsView * view;
	GraphicsScene * m_graphicsScene;
};
