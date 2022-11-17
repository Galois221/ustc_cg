#pragma once

#include "Figure.h"

class Line : public Figure {
public:
	Line();
	~Line();

	void Draw(QPainter& painter);

};
