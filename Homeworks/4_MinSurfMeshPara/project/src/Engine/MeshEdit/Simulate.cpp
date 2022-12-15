#include <Engine/MeshEdit/Simulate.h>


using namespace Ubpa;

using namespace std;
using namespace Eigen;
typedef Eigen::Triplet<double> T;

void Simulate::Clear() {
	this->positions.clear();
	this->velocity.clear();
}

bool Simulate::Init() {
	//Clear();
	int n = this->positions.size();

	is_fixed.resize(n);
	for (int i = 0; i < n; i++) is_fixed[i] = false;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < fixed_id.size(); j++) {
			if (fixed_id[j] == i) is_fixed[i] = true;
		}
	}

	GetInit_length();


	//Set f_ext
	this->velocity.resize(3 * n);
	f_ext.resize(3 * n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->velocity[i][j] = 0;
		}
		this->f_ext[3 * i] = 9.8 / 1.414;
		this->f_ext[3 * i + 1] = 0;
		this->f_ext[3 * i + 2] = 9.8 / 1.414;
	}
	
	//set coff matrix
	SetCoffMatrix();
	
	//Init Matrix J
	J.resize(3 * n, 3 * (edgelist.size() / 2));
	vector<T> J_trip;
	for (size_t i = 0; i < this->edgelist.size() / 2; i++) {
		int a = edgelist[2 * i];
		int b = edgelist[2 * i + 1];
		for (int j = 0; j < 3; j++) {
			J_trip.push_back(T(3 * a + j, 3 * i + j, stiff));
			J_trip.push_back(T(3 * b + j, 3 * i + j, -stiff));
		}
	}
	J.setFromTriplets(J_trip.begin(), J_trip.end());
	
	return true;
}

bool Simulate::Run() {
	if (method == 1) { 
		EulerImplicit_Method(); 
		cout << "Euler Implicit Method!";
	}
	else if (method == 2) { 
		Fast_Method(); 
		cout << "Fast Method!";
	}
	else cout << "ERROR:Unknown Method!" << endl;
	// half-edge structure -> triangle mesh

	return true;
}

void Ubpa::Simulate::SetLeftFix()
{
	//固定网格x坐标最小点
	fixed_id.clear();
	double x = 100000;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][0] < x)
		{
			x = positions[i][0];
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (abs(positions[i][0] - x) < 1e-5)
		{
			fixed_id.push_back(i);
		}
	}

	Init();
}

void Simulate::EulerImplicit_Method() {

	size_t n = positions.size();
	VectorXd y = VectorXd::Zero(3 * n);
	VectorXd X0(3 * n);

	//Get y and Init f_int
	f_int = VectorXd::Zero(3 * n);
	for (size_t i = 0; i < n; i++) {
		for (size_t j = 0; j < 3; j++) {
			if (is_fixed[i]) {
				y(3 * i + j) = positions[i][j];
			}
			else {
				y(3 * i + j) = positions[i][j] + h * velocity[i][j] + h * h * f_ext(3 * i + j) / M;
			}
		}
	}
	//Init X0
	X0 = y;
	//Euler Implicit Method
	int times = 0;
	while (times < 5) {
		vector<T> tripletlist;

		//Set f_int and delta g
		for (size_t i = 0; i < edgelist.size() / 2; i++) {
			int a = edgelist[2 * i];
			int b = edgelist[2 * i + 1];
			Vector3d f1;
			for (int k = 0; k < 3; k++) f1(k) = X0(3 * b + k) - X0(3 * a + k);
			double cof = stiff * (1 - init_length[i] / f1.norm());

			for (int k = 0; k < 3; k++) { 
				f_int(3 * a + k) += cof * f1(k);
				f_int(3 * b + k) -= cof * f1(k);
			}

			//------delta g -------
			Matrix3d I = Matrix3d::Identity();
			Matrix3d f_x1 = -cof * I - stiff * init_length[i] / pow(f1.norm(), 3) * f1 * f1.transpose();
			if (!is_fixed[a]) {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						tripletlist.push_back(T(3 * a + i, 3 * b + j, h * h * f_x1(i, j)));
						tripletlist.push_back(T(3 * a + i, 3 * a + j, -h * h * f_x1(i, j)));
					}
				}
			}
			if (!is_fixed[b]) {
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						tripletlist.push_back(T(3 * b + i, 3 * a + j, h * h * f_x1(i, j)));
						tripletlist.push_back(T(3 * b + i, 3 * b + j, -h * h * f_x1(i, j)));
					}
				}
			}
		}
		for (size_t k = 0; k < 3 * n; k++) tripletlist.push_back(T(k, k, M));

		//set Vector g
		VectorXd g = VectorXd::Zero(3 * n);
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < 3; j++) {
				if (is_fixed[i]) {
					g(3 * i + j) = 0;
				}
				else {
					g(3 * i + j) = M * (X0(3 * i + j) - y(3 * i + j)) - h * h * f_int(3 * i + j);
				}
			}
		}

		//solve
		SparseMatrix<double> DG(3 * n, 3 * n);
		DG.setFromTriplets(tripletlist.begin(), tripletlist.end());
		DG.makeCompressed();
		SparseLU<SparseMatrix<double>> solver;
		solver.compute(DG);
		VectorXd DX = solver.solve(g);
		X0 -= DX;
		times++;
	}
	//set result
	for (size_t i = 0; i < n; i++) {
		if (!is_fixed[i]) {
			for (int j = 0; j < 3; j++) {
				velocity[i][j] = (X0(3 * i + j) - positions[i][j]) / h;
				positions[i][j] = X0(3 * i + j);
			}
		}
	}
}

void Simulate::Fast_Method() {
	size_t n = positions.size();
	size_t s = edgelist.size() / 2;
	VectorXd X, d, y0;
	//Init y0 and X
	y0.resize(3 * n);
	X.resize(3 * n);
	for (size_t i = 0; i < n; i++) {
		for (int j = 0; j < 3; j++) {
			y0(3 * i + j) = positions[i][j] + h * velocity[i][j] + h * h * f_ext(3 * i + j) / M;
			X(3 * i + j) = positions[i][j];
		}
	}

	//Init d
	d.resize(3 * s);
	for (size_t i = 0; i < s; i++) {
		pointf3 p1, p2;
		int a = edgelist[2 * i];
		int b = edgelist[2 * i + 1];
		p1 = positions[a];
		p2 = positions[b];
		for (int j = 0; j < 3; j++) {
			d(3 * i + j) = init_length[i] * (p1[j] - p2[j]) / (p1 - p2).norm();
		}
	}

	//Fast method
	int times = 0;
	while (times < 10) {
		VectorXd Xf = Coff_LU.solve(K * (h * h * J * d + M * y0 - Mh2L * b));

		//update X
		size_t j = 0;
		for (size_t i = 0; i < n; i++) {
			if (!is_fixed[i]) {
				for (int k = 0; k < 3; k++) {
					X(3 * i + k) = Xf(3 * j + k);
				}
				j++;
			}
		}

		//update d
		for (size_t i = 0; i < s; i++) {
			pointf3 p1, p2;
			int a = edgelist[2 * i];
			int b = edgelist[2 * i + 1];
			for (int k = 0; k < 3; k++) {
				p1[k] = X(3 * a + k);
				p2[k] = X(3 * b + k);
			}
			for (int j = 0; j < 3; j++) {
				d(3 * i + j) = init_length[i] * (p1[j] - p2[j]) / (p1 - p2).norm();
			}
		}
		times++;
	}

	//Set result
	for (int i = 0; i < n; i++) {
		if (!is_fixed[i]) {
			for (int j = 0; j < 3; j++) {
				velocity[i][j] = (X(3 * i + j) - positions[i][j]) / h;
				positions[i][j] = X(3 * i + j);
			}
		}
	}
}

void Simulate::GetInit_length() {
	init_length.resize(this->edgelist.size() / 2);
	for (size_t i = 0; i < this->edgelist.size() / 2; i++) {
		int a = edgelist[2 * i];
		int b = edgelist[2 * i + 1];
		init_length[i] = (positions[a] - positions[b]).norm();
	}
}


void Simulate::SetCoffMatrix() {
	//Set Matirx K
	size_t n = positions.size();
	size_t m = positions.size() - fixed_id.size();
	vector<T> trip;
	b = VectorXd::Zero(3 * n);
	K.resize(3 * m, 3 * n);
	int j = 0;
	for (size_t i = 0; i < n; i++) {
		if (!is_fixed[i]) {
			for (int k = 0; k < 3; k++) trip.push_back(T(3 * j + k, 3 * i + k, 1));
			j++;
		}
		else {
			for (int k = 0; k < 3; k++) b(3 * i + k) = positions[i][k];
		}
	}
	K.setFromTriplets(trip.begin(), trip.end());

	//Set Mh2L
	Mh2L.resize(3 * n, 3 * n);
	vector<T> trip2;
	for (size_t i = 0; i < edgelist.size() / 2; i++) {
		int a = edgelist[2 * i];
		int b = edgelist[2 * i + 1];
		for (int j = 0; j < 3; j++) {
			trip2.push_back(T(3 * a + j, 3 * a + j, h * h * stiff));
			trip2.push_back(T(3 * b + j, 3 * b + j, h * h * stiff));
			trip2.push_back(T(3 * a + j, 3 * b + j, -h * h * stiff));
			trip2.push_back(T(3 * b + j, 3 * a + j, -h * h * stiff));
		}
	}
	for (size_t i = 0; i < 3 * n; i++) trip2.push_back(T(i, i, M));
	Mh2L.setFromTriplets(trip2.begin(), trip2.end());

	//set Coff
	if (fixed_id.size() > 0) Coff = K * Mh2L * K.transpose();
	else Coff = Mh2L;

	//Set Solver
	Coff.makeCompressed();
	Coff_LU.compute(Coff);
}

