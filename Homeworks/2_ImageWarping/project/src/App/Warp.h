#pragma once
#include <QWidget>
#include <QLine>
#include <QImage>
#include <QVector>
#include <Eigen/Dense>

using namespace Eigen;

#define EPSILON 1e-16

class Warp
{

public:
	Warp();
	void InitPoints(QVector<QLine*>&data_pair_);
	void imageWarp(QImage* image_, int setgap);
	virtual Vector2d Warp_f(const QPoint&p)const;
	virtual ~Warp();
protected:
	QVector<QLine*> line_array_; //initial points and target points
	QVector<QPoint> p_, q_;
	double dist(const QPoint& p1, const QPoint& p2)const;
private:
	Vector2d cooridinates(Vector2d o,Vector2d A,Vector2d B,int x ,int y) const;
	int isInTriangle(Vector2d A, Vector2d B, Vector2d C, int x, int y) const;	// judge if the point (x, y) is in the square ABC
};