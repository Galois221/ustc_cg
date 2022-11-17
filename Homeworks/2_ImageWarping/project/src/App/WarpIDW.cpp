#include"WarpIDW.h"

WarpIDW::WarpIDW(QVector<QLine*>& data_pair_)
{
	InitPoints(data_pair_);

	//calculate T[i]
	T = QVector<Matrix2d>(p_.size());
	for (int i = 0; i < p_.size(); i++)
	{
		Matrix2d A;
		Vector2d b1, b2, t1, t2;
		A.fill(0.0);
		b1.fill(0.0);
		b2.fill(0.0);
		for (int j = 0; j < p_.size(); j++)
		{
			if (j == i)
			{
				continue;  //跳过当前循环
			}
			int dpx = (p_[j] - p_[i]).x();
			int dpy = (p_[j] - p_[i]).y();
			int dqx = (q_[j] - q_[i]).x();
			int dqy = (q_[j] - q_[i]).y();
			double cof = inverse_dist(i, p_[j]);

			A(0, 0) += dpx * dpx * cof;
			A(0, 1) += dpx * dpy * cof;
			A(1, 0) = A(0, 1);
			A(1, 1) += dpy * dpy * cof;

			b1[0] += dpx * dqx * cof;  //系数？
			b1[1] += dpy * dqx * cof;

			b2[0] += dpx * dqy * cof;
			b2[1] += dpy * dqy * cof;
		}
		t1 = A.lu().solve(b1);
		t2 = A.lu().solve(b2);
		T[i](0, 0) = t1[0];
		T[i](0, 1) = t1[1];
		T[i](1, 0) = t2[0];
		T[i](1, 1) = t2[1];
	}
}

double WarpIDW::inverse_dist(int i, const QPoint& p) const
{
	assert(0 <= i && i < p_.size());
	double d = dist(p_[i], p);
	if (fabs(d) < EPSILON) return 0.0; //被除数不能太小
	else return 1.0 / pow(d,2);
}

Vector2d WarpIDW::local_approximation_f(int i, const QPoint& p) const
{
	assert(0 <= i && i < p_.size());
	QPoint diff = p - p_[i], qi = q_[i];
	Vector2d ans{ 0,0 };
	ans[0] = T[i](0,0)*diff.x()+T[i](0,1)*diff.y()+qi.x();
	ans[1] = T[i](1, 0) * diff.x() + T[i](1, 1) * diff.y() + qi.y();
	return ans;
}

double WarpIDW::w_cof(int i, const QPoint& p) const
{
	assert(0 <= i && i < p_.size());

	double sum = 0.0;
	for (int j = 0; j < p_.size(); j++)
	{
		if (p == p_[j])
		{
			return i == j ? 1.0 : 0.0;
		}
		else
		{
			sum += inverse_dist(j, p);
		}
	}

	assert(fabs(sum) >= EPSILON);	// otherwise invert_dist(i,p) <= W_EPSILON, which means p == p_[i] and the function would have returned
	return inverse_dist(i, p) / sum;
}

Vector2d WarpIDW::Warp_f(const QPoint&p) const
{
	Vector2d ans{ 0,0 };
	for (int i = 0; i < p_.size(); i++)
		ans += w_cof(i, p) * local_approximation_f(i, p);
	return ans;
}

WarpIDW::~WarpIDW()
{
}