#ifndef INCLUDED_LIGHT_H
#define INCLUDED_LIGHT_H

#include <QVector3D>

class Light
{
public:
	Light();
	Light(QVector3D color, QVector3D location);
	QVector3D GetColor() { return m_color; }
	QVector3D GetLocation() { return m_location; }

private:
	QVector3D m_color;
	QVector3D m_location;

};


#endif