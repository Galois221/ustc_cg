#pragma once
#include<Eigen/Sparse>
#include<Eigen/Dense>
using namespace Eigen;
typedef Eigen::SparseMatrix<double> SparseMatrixType;
typedef Eigen::Triplet<double> T;
typedef Eigen::SimplicialCholesky<SparseMatrixType> Solver;

class Poisson
{
public:
	Poisson();
	~Poisson();
	VectorXd Poisson_solver(MatrixXd f, MatrixXd g, MatrixXd selected, Solver* p_A, bool mixed);
	Solver* Get_Poisson_solver(MatrixXd selected);
};