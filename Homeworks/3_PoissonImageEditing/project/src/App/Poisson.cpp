#include"Poisson.h"
#include<iostream>
#include<Eigen/Sparse>
#include<Eigen/Dense>

using namespace std;
typedef Eigen::SparseMatrix<double> SparseMatrixType;
typedef Eigen::Triplet<double> T;
typedef Eigen::SimplicialCholesky<SparseMatrixType> Solver;

Poisson::Poisson()
{
}

Poisson::~Poisson()
{
}


Solver* Poisson::Get_Poisson_solver(MatrixXd selected)
{
	int height = (int)selected.rows();
	int width = (int)selected.cols();
	int N = height * width;
	SparseMatrix<double> A(N, N);
	vector<T> tripletlist;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++) {
			int c = i * width + j;
			if (selected(i, j) == 0)
			{
				tripletlist.push_back(T(c, c, 1));
			}
			else
			{
				tripletlist.push_back(T(c, c, 4));
				if (selected(i - 1, j) == 1)
					tripletlist.push_back(T(c, c - width, -1));
				if (selected(i + 1, j) == 1)
					tripletlist.push_back(T(c, c + width, -1));
				if (selected(i, j - 1) == 1)
					tripletlist.push_back(T(c, c - 1, -1));
				if (selected(i, j + 1) == 1)
					tripletlist.push_back(T(c, c + 1, -1));
			}
		}
	}
	A.setFromTriplets(tripletlist.begin(), tripletlist.end());
	A.makeCompressed();

	Solver* p_A = new Solver(A);
	return p_A;
}

VectorXd Poisson::Poisson_solver(MatrixXd f, MatrixXd g, MatrixXd selected, Solver* p_A, bool mixed)
{
	int height = (int)f.rows();
	int width = (int)f.cols();
	int N = height * width;
	VectorXd b = VectorXd::Zero(N);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++) {
			int c = i * width + j;
			if (selected(i, j) == 0)
			{
				b(c) = f(i, j);
			}
			else
			{
				if (selected(i - 1, j) != 1)
					b(c) += f(i - 1, j);
				if (selected(i + 1, j) != 1)
					b(c) += f(i + 1, j);
				if (selected(i, j - 1) != 1)
					b(c) += f(i, j - 1);
				if (selected(i, j + 1) != 1)
					b(c) += f(i, j + 1);
				if (mixed)
				{
					vector<double> tmp(4);
					tmp[0] = g(i, j) - g(i + 1, j);
					tmp[1] = g(i, j) - g(i - 1, j);
					tmp[2] = g(i, j) - g(i, j + 1);
					tmp[3] = g(i, j) - g(i, j - 1);
					if (fabs(tmp[0]) < fabs(f(i, j) - f(i + 1, j))) tmp[0] = f(i, j) - f(i + 1, j);
					if (fabs(tmp[1]) < fabs(f(i, j) - f(i - 1, j))) tmp[1] = f(i, j) - f(i - 1, j);
					if (fabs(tmp[2]) < fabs(f(i, j) - f(i, j + 1))) tmp[2] = f(i, j) - f(i, j + 1);
					if (fabs(tmp[3]) < fabs(f(i, j) - f(i, j - 1))) tmp[3] = f(i, j) - f(i, j - 1);
					b(c) += tmp[0] + tmp[1] + tmp[2] + tmp[3];
				}
				else
				{
					b(c) += 4 * g(i, j) - g(i + 1, j) - g(i - 1, j) - g(i, j + 1) - g(i, j - 1);
				}
			}
		}
	}
	VectorXd x;
	x = p_A->solve(b);
	return x;
}
