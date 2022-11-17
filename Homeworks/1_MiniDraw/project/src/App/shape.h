#pragma once

#include <QtGui>

class Shape
{
public:
	Shape();
	virtual ~Shape(); //���麯��
	virtual void Draw(QPainter& paint) = 0;//�麯��
	void set_start(QPoint s);
	void set_end(QPoint e);

public:
	enum Type
	{
		kDefault = 0,
		kLine = 1,
		kRect = 2,
	};

protected:
	QPoint start;
	QPoint end;
};

