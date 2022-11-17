#pragma once
#include<iostream>
#include<QtGui>

class Figure
{
public:
	Figure() {};
	virtual void Draw(QPainter& paint)=0;
	void set_start_point_(QPoint a);
	void set_end_point_(QPoint b);
	virtual ~Figure();
public:
	QVector<QPoint> points;
protected:
	QPoint start_point_;
	QPoint end_point_;

};