#pragma once
#include"Figure.h"

class Polygonn : public Figure {
public:
	Polygonn();
	~Polygonn();

	void Draw(QPainter& painter);
};
