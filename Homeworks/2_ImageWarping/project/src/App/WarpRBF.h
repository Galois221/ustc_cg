#pragma once
#include"Warp.h"


class WarpRBF :public Warp
{
private:
	Vector2d b;
	Matrix2d A;  //rotate Matrix
	Matrix<double, Dynamic, 2> U;

public:
	WarpRBF(QVector<QLine*>& data_pair_);
	~WarpRBF();
	double basis_function_i(double d,int i) const;

private:
	Vector2d Warp_f(const QPoint& p) const;
};