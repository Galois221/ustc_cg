#include"Ellipse.h"
#include <iostream>

Ellipsee::Ellipsee(void) {};

Ellipsee::~Ellipsee(void) {};

void Ellipsee::Draw(QPainter& paint)
    {
        paint.drawEllipse(start_point_.x(), start_point_.y(), end_point_.x() - start_point_.x(), end_point_.y() - start_point_.y());
    }
