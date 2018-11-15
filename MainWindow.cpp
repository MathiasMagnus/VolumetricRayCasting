// Raycaster includes
#include "mainwindow.hpp"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	view = new QGraphicsView;
	m_graphicsScene = new GraphicsScene();
	view->setScene(m_graphicsScene);
	this->setCentralWidget(view);
}

MainWindow::~MainWindow()
{
	delete view;
	delete m_graphicsScene;
}


void MainWindow::Raycast()
{
	m_graphicsScene->Raycast();
	//view->show(); 
}
