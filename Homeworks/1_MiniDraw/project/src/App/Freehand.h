#pragma once
#include"Figure.h"

class Freehand : public Figure
{
public:
	Freehand();
	~Freehand();

	void Draw(QPainter& painter);
};