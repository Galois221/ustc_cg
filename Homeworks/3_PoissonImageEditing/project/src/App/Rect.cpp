#include "Rect.h"

Rect::Rect(){};
Rect::~Rect(){};

void Rect::Draw(QPainter& painter)
{
	painter.drawRect(start_point_.x(), start_point_.y(), end_point_.x() - start_point_.x(), end_point_.y() - start_point_.y());
}
