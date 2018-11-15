#pragma once

// Raycaster includes
#include "Raycaster.hpp"

// Qt includes
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>


class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit GraphicsScene(QObject *parent = 0);
	~GraphicsScene();
	virtual void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);
	virtual	void mousePressEvent(QGraphicsSceneMouseEvent *mousePressEvent);
	virtual	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseMoveEvent);
	virtual	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseReleaseEvent);
	virtual void keyPressEvent(QKeyEvent *event);
//	virtual	void keyPressEvent(QKeyEvent *event);
	
	void Raycast();

	template <typename RC>
	void SetRayCaster(RC&& rc, const Camera& camera)
	{
		m_ray_caster = rc;
		m_camera = camera;
	}

	std::function< void (QImage&, int, int, const Camera&)> m_ray_caster;

	QPointF origPoint;
	int m_imageWidth ;
	int m_imageHeight ;
	Camera m_camera;

signals:

	public slots :

private:
	QGraphicsPixmapItem *m_graphicsPixmapItem;

};
