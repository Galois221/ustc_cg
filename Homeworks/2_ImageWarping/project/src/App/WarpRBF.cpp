#include"WarpRBF.h"

WarpRBF::WarpRBF(QVector<QLine*>& data_pair_) {
	InitPoints(data_pair_);

	Matrix<double, Dynamic, 3> X;
	Matrix<double, Dynamic, 2> Y;
	X.resize(p_.size(), 3);
	Y.resize(p_.size(), 2);
	for (int i = 0; i < p_.size(); i++)
	{
		X(i, 0) = 1;
		X(i, 1) = p_[i].x();
		X(i, 2) = p_[i].y();
		Y(i, 0) = q_[i].x();
		Y(i, 1) = q_[i].y();
	}
	if (p_.size() == 1) {
		A = Matrix2d::Identity();
		b.x() = q_[0].x() - p_[0].x();
		b.y() = q_[0].y() - p_[0].y();
	}
	if (p_.size() == 2) {
		A(1, 0) = 0;
		A(0, 1) = 0;
		A(0, 0) = (p_[1] - p_[0]).x() == 0 ? 1 : double((q_[1] - q_[0]).x()) / (p_[1] - p_[0]).x();
		A(1, 1) = (p_[1] - p_[0]).y() == 0 ? 1 : double((q_[1] - q_[0]).y()) / (p_[1] - p_[0]).y();
		b[0] = q_[0].x() - A(0, 0) * p_[0].x();
		b[1] = q_[1].y() - A(1, 1) * p_[1].y();
	}
	else {
		Matrix<double, 3, 2> TT;
		TT.fill(0.0);
		TT = (X.transpose() * X).inverse() * X.transpose() * Y;
		b[0] = TT(0, 0);
		b[1] = TT(0, 1);
		A(0, 0) = TT(1, 0);
		A(0, 1) = TT(2, 0);
		A(1, 0) = TT(1, 1);
		A(1, 1) = TT(2, 1);
	}
	Matrix<double, Dynamic, Dynamic> G;
	Matrix<double, Dynamic, 2> F;
	G.resize(p_.size(), p_.size());
	F.resize(p_.size(), 2);
	U.resize(p_.size(), 2);

	Vector2d tmp;

	for (int i = 0; i < p_.size(); i++)
	{
		for (int j = 0; j < p_.size(); j++)
		G(i, j) = basis_function_i(dist(p_[i], p_[j]), j);
		tmp = Vector2d(q_[i].x(), q_[i].y()) - A * Vector2d(p_[i].x(), p_[i].y()) - b;
		F(i, 0) = tmp[0];
		F(i, 1) = tmp[1];
	}

	U = G.colPivHouseholderQr().solve(F);
}

WarpRBF::~WarpRBF() {

}

double WarpRBF::basis_function_i(double d,int i) const
{
	double r;
	if (p_.size() == 1) r = 1;
	else {
		r = i != 0 ? dist(p_[i], p_[0]) : dist(p_[i], p_[1]);
		for (int j = 0; j < p_.size(); j++) {
			if (j != i) {
				double tmp = dist(p_[j], p_[i]);
				r = tmp < r ? tmp : r;
			}
		}
	}
	double b = d * d + r * r;
	return pow(b, 0.5);
}


Vector2d WarpRBF::Warp_f(const QPoint& p) const
{
	Vector2d ans{ 0,0 };
	Vector2d x{p.x(),p.y()};
	ans += A* x+b;
	for (int i = 0; i < p_.size(); i++)
		ans += Vector2d(U(i,0),U(i,1)) * basis_function_i(dist(p, p_[i]), i);
	return ans;
}