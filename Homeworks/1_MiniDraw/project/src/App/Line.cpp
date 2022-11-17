#include "Line.h"


Line::Line(){};

Line::~Line(){};

void Line::Draw(QPainter& painter)
{
painter.drawLine(start_point_.rx(), start_point_.ry(), end_point_.rx(), end_point_.ry());//ªÊ÷∆÷±œﬂ
}
