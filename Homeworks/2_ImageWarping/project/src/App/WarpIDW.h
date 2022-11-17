#pragma once
#include"Warp.h"

class WarpIDW : public Warp
{
private:
	QVector<Matrix2d> T; // the local approcimation matrix

public:
	WarpIDW(QVector<QLine*>& data_pair_);
	~WarpIDW();
	
private:
	double inverse_dist(int i, const QPoint& p) const; //calculate the inverse distance
	double w_cof(int i, const QPoint& p)const; //wi
	Vector2d local_approximation_f(int i, const QPoint& p) const; //fi
	Vector2d Warp_f(const QPoint&p) const;
};