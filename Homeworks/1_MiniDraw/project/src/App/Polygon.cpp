#include"Polygon.h"

Polygonn::Polygonn() {};

Polygonn::~Polygonn() {};

void Polygonn::Draw(QPainter& painter) {
	for (int i = 0; i < points.size()-1; i++) painter.drawLine(points[i], points[i + 1]);
	painter.drawLine(points[points.size()-1], points[0]);
}