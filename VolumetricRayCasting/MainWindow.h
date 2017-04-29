#ifndef VolumetricRayCasting_H
#define VolumetricRayCasting_H

#include <QtWidgets/QMainWindow>
#include "ui_VolumetricRayCasting.h"
#include "SceneData.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include "GraphicsScene.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	
	void SetScene(const SceneData &sceneData);
	void Raycast();

private:
	Ui::VolumetricRayCastingClass ui;
	QGraphicsView * view;
	GraphicsScene * scene;
};

#endif // VolumetricRayCasting_H
