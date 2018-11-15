// Raycaster includes
#include "GraphicsScene.h"


GraphicsScene::GraphicsScene(QObject *parent) :
	QGraphicsScene(parent)
{
	m_graphicsPixmapItem = new QGraphicsPixmapItem();
	addItem(m_graphicsPixmapItem);

	m_imageWidth = 400;
	m_imageHeight = 300;
}

GraphicsScene::~GraphicsScene()
{
	delete m_graphicsPixmapItem;
}

void GraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
	int delta = wheelEvent->delta() / 120; 
	glm::vec3 newCameraPos = m_camera.GetPosition();
	/*newCameraPos.setZ(newCameraPos.z() + newCameraPos.z() / (10.0 * delta * copysignf(1.0, m_camera.GetViewDir().z())));
	newCameraPos.setY(newCameraPos.y() + newCameraPos.y() / (10.0 * delta * copysignf(1.0, m_camera.GetViewDir().y())));
	newCameraPos.setX(newCameraPos.x() + newCameraPos.x() / (10.0 * delta * copysignf(1.0, m_camera.GetViewDir().x())));*/
	
	newCameraPos = glm::vec3(newCameraPos.x + newCameraPos.x / (10.0 * delta * copysignf(1.0, m_camera.GetViewDir().x)),
		newCameraPos.y + newCameraPos.y / (10.0 * delta * copysignf(1.0, m_camera.GetViewDir().y)),
		newCameraPos.z + newCameraPos.z / (10.0 * delta * copysignf(1.0, m_camera.GetViewDir().z)));
	m_camera.SetPosition(newCameraPos);
	Raycast();
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mousePressEvent)
{
	origPoint = mousePressEvent->scenePos();
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseMoveEvent)
{
	if (Qt::LeftButton == mouseMoveEvent->buttons())
	{
		QPointF movingPoint = mouseMoveEvent->scenePos();
		QPointF delta = movingPoint - origPoint;
		
		if (delta.x() > 0)
		{
				m_camera.RotateAroundYClock();
		}
		else if (delta.x() < 0 )
		{
				m_camera.RotateAroundYCClock();
		}
		
		if (delta.y() > 0 )
		{
				m_camera.RotateAroundXClock();		
		}

		else if (delta.y() < 0)
		{
				m_camera.RotateAroundXCClock();
		}

		origPoint = movingPoint;
		Raycast();	
	}
}
void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseReleaseEvent)
{
}

void GraphicsScene::keyPressEvent(QKeyEvent *event)
{
	
	if ( Qt::Key_Up  == event->key())
		m_camera.RotateAroundXCClock();
	else if (Qt::Key_Down == event->key())
		m_camera.RotateAroundXClock();
	else if (Qt::Key_Left == event->key())
		m_camera.RotateAroundYCClock();
	else if (Qt::Key_Right == event->key())
		m_camera.RotateAroundYClock();
	
	Raycast();
}


void GraphicsScene::Raycast() 
{
	QImage resultsImage(m_imageWidth, m_imageHeight, QImage::Format_RGBA8888);
	m_ray_caster(resultsImage, m_imageWidth, m_imageHeight, m_camera);
	QPixmap pix = QPixmap::fromImage(resultsImage); // Create pixmap from image
	m_graphicsPixmapItem->setPixmap(pix);
}

