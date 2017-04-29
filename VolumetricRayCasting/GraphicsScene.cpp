#include "GraphicsScene.h"


GraphicsScene::GraphicsScene(QObject *parent) :
	QGraphicsScene(parent)
{
	m_graphicsPixmapItem = new QGraphicsPixmapItem();
	addItem(m_graphicsPixmapItem);
}

GraphicsScene::~GraphicsScene()
{
	delete m_graphicsPixmapItem;
}

void GraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
	int delta = wheelEvent->delta() / 120; 
	QVector3D newCameraPos = rayCastScene.GetCameraPos();
	newCameraPos.setZ(newCameraPos.z() + newCameraPos.z() / (10.0 * delta * copysignf(1.0, rayCastScene.GetCameraViewDir().z())));
	newCameraPos.setY(newCameraPos.y() + newCameraPos.y() / (10.0 * delta * copysignf(1.0, rayCastScene.GetCameraViewDir().y())));
	newCameraPos.setX(newCameraPos.x() + newCameraPos.x() / (10.0 * delta * copysignf(1.0, rayCastScene.GetCameraViewDir().x())));
	
	RaycastFromCameraPos(newCameraPos);

	//QGraphicsScene::wheelEvent(wheelEvent); // do I need this?
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mousePressEvent)
{
	origPoint = mousePressEvent->scenePos();
	
	//QGraphicsScene::mousePressEvent(mousePressEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseMoveEvent)
{
	if (Qt::LeftButton == mouseMoveEvent->buttons())
	{
		QPointF movingPoint = mouseMoveEvent->scenePos();
		QPointF delta = movingPoint - origPoint;
		
		if (delta.x() > 0)
		{
			//for (int i = 0; i < std::abs(int(delta.x())); i++)
				rayCastScene.m_camera.RotateAroundYClock();
		}
		else if (delta.x() < 0 )
		{
			//for (int i = 0; i < std::abs(int(delta.x())); i++)
				rayCastScene.m_camera.RotateAroundYCClock();
		}
		
		if (delta.y() > 0 )
		{
			//for (int i = 0; i < std::abs(int(delta.y())); i++)
				rayCastScene.m_camera.RotateAroundXClock();		
		}

		else if (delta.y() < 0)
		{
			//for (int i = 0; i < std::abs(int(delta.y())); i++)
				rayCastScene.m_camera.RotateAroundXCClock();
		}

		origPoint = movingPoint;
		Raycast();	
	}

	//GraphicsScene::mouseMoveEvent(mouseMoveEvent); // why does it crash if I call this at the end?
	
}
void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseReleaseEvent)
{
}

static int kkk = 0;
void GraphicsScene::keyPressEvent(QKeyEvent *event)
{
	kkk++;
	std::cout << kkk << endl;
	if ( Qt::Key_Up  == event->key())
		rayCastScene.m_camera.RotateAroundXCClock();
	else if (Qt::Key_Down == event->key())
		rayCastScene.m_camera.RotateAroundXClock();
	else if (Qt::Key_Left == event->key())
		rayCastScene.m_camera.RotateAroundYCClock();
	else if (Qt::Key_Right == event->key())
		rayCastScene.m_camera.RotateAroundYClock();
			
	Raycast();
}

void GraphicsScene::SetScene(const SceneData &sceneData)
{
	// Init with camera position, viewed posisiton, field of view
	rayCastScene.Init(sceneData.cameraPositionData, sceneData.viewedPosisitonData, sceneData.fieldOfViewData);
	rayCastScene.SetLimits(sceneData.extentData);
	rayCastScene.m_densityFunc = sceneData.functionData;
}

void GraphicsScene::Raycast() 
{
	//Raycast scene
	rayCastScene.Raycast();
	const std::vector<unsigned char> rgbData = rayCastScene.GetRGBData();

	const uchar* pixDataRGB = &rgbData[0];
	// 400 pixels width, 300 pixels height, x bytes per line, RGB888 format
	QImage img(pixDataRGB, 400, 300, 400 * 3, QImage::Format_RGB888); 															
	QPixmap pix = QPixmap::fromImage(img); // Create pixmap from image
	m_graphicsPixmapItem->setPixmap(pix);
}

void GraphicsScene::RaycastFromCameraPos(const QVector3D &cameraPosition)
{
	rayCastScene.SetCameraPos(cameraPosition);
	Raycast();
}

