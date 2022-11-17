#include"Freehand.h"

Freehand::Freehand() {};
Freehand::~Freehand() {};

void Freehand::Draw(QPainter&painter)
{
	for (int i = 1; i < points.size(); i++)
		painter.drawLine(points[i-1], points[i]);
}
