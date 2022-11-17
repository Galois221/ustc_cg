#pragma once

#include "Figure.h"

class Rect : public Figure {
public:
	Rect();
	~Rect();

	void Draw(QPainter& painter);
};

