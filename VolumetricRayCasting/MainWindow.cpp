#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	view = new QGraphicsView;
	scene = new GraphicsScene();
	view->setScene(scene);
	this->setCentralWidget(view);
}

MainWindow::~MainWindow()
{
	delete view;
	delete scene;
}

void MainWindow::SetScene(const SceneData &sceneData)
{
	scene->SetScene(sceneData);
}

void MainWindow::Raycast()
{
	scene->Raycast();
	//view->show(); 
}