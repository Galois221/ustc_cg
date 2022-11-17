#pragma once

#include"Figure.h"

class Ellipsee : public Figure
{
public:
    Ellipsee(void);
    ~Ellipsee(void);

public:
    void Draw(QPainter& paint);
};