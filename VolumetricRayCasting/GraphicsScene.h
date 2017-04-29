#pragma once

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

#include "SceneData.h"
#include "Scene.h"


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

	void SetScene(const SceneData &sceneData);
	void Raycast();
	void RaycastFromCameraPos(const QVector3D &cameraPosition);
	Scene rayCastScene;
	QPointF origPoint;
signals:

	public slots :

private:
	QGraphicsPixmapItem *m_graphicsPixmapItem;

};