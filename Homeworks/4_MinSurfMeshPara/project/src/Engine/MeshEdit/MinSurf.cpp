#include <Engine/MeshEdit/MinSurf.h>

#include <Engine/Primitive/TriMesh.h>

#include <Eigen/Sparse>

using namespace Ubpa;

using namespace std;
using namespace Eigen;
typedef Eigen::Triplet<double> T;

MinSurf::MinSurf(Ptr<TriMesh> triMesh)
	: heMesh(make_shared<HEMesh<V>>())
{
	Init(triMesh);
}

void MinSurf::Clear() {
	heMesh->Clear();
	triMesh = nullptr;
}

bool MinSurf::Init(Ptr<TriMesh> triMesh) {
	Clear();

	if (triMesh == nullptr)
		return true;

	if (triMesh->GetType() == TriMesh::INVALID) {
		printf("ERROR::MinSurf::Init:\n"
			"\t""trimesh is invalid\n");
		return false;
	}

	// init half-edge structure
	size_t nV = triMesh->GetPositions().size();
	vector<vector<size_t>> triangles;
	triangles.reserve(triMesh->GetTriangles().size());
	for (auto triangle : triMesh->GetTriangles())
		triangles.push_back({ triangle->idx[0], triangle->idx[1], triangle->idx[2] });
	heMesh->Reserve(nV);
	heMesh->Init(triangles);

	if (!heMesh->IsTriMesh() || !heMesh->HaveBoundary()) {
		printf("ERROR::MinSurf::Init:\n"
			"\t""trimesh is not a triangle mesh or hasn't a boundaries\n");
		heMesh->Clear();
		return false;
	}

	// triangle mesh's positions ->  half-edge structure's positions
	for (int i = 0; i < nV; i++) {
		auto v = heMesh->Vertices().at(i);
		v->pos = triMesh->GetPositions()[i].cast_to<vecf3>();
	}

	this->triMesh = triMesh;
	return true;
}

bool MinSurf::Run() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::MinSurf::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	Minimize();

	// half-edge structure -> triangle mesh
	size_t nV = heMesh->NumVertices();
	size_t nF = heMesh->NumPolygons();
	vector<pointf3> positions;
	vector<unsigned> indice;
	positions.reserve(nV);
	indice.reserve(3 * nF);
	for (auto v : heMesh->Vertices())
		positions.push_back(v->pos.cast_to<pointf3>());
	for (auto f : heMesh->Polygons()) { // f is triangle
		for (auto v : f->BoundaryVertice()) // vertices of the triangle
			indice.push_back(static_cast<unsigned>(heMesh->Index(v)));
	}

	triMesh->Init(indice, positions);

	return true;
}

void MinSurf::Minimize() {
	size_t nV = heMesh->NumVertices();
	vector<T> tripletlist;
	VectorXd b_x = VectorXd::Zero(nV);
	VectorXd b_y = VectorXd::Zero(nV);
	VectorXd b_z = VectorXd::Zero(nV);

	for (auto v : heMesh->Vertices()) {
		int i = static_cast<int>(heMesh->Index(v));
		// if v_i is in the boundary
		if (v->IsBoundary()) {
			b_x(i) += v->pos[0];
			b_y(i) += v->pos[1];
			b_z(i) += v->pos[2];
			tripletlist.push_back(T(i, i, 1));
			continue;
		}
		//if v_i is not in boundary
		tripletlist.push_back(T(i, i, v->Degree()));
		for (auto n : v->AdjVertices()) {
			int j = static_cast<int>(heMesh->Index(n));
			if (n->IsBoundary()) {
				b_x(i) += n->pos[0];
				b_y(i) += n->pos[1];
				b_z(i) += n->pos[2];
			}
			else
			{
				tripletlist.push_back(T(i, j, -1));
			}
		}
	}
	SparseMatrix<double> A(nV, nV);
	A.setFromTriplets(tripletlist.begin(), tripletlist.end());
	A.makeCompressed();

	SimplicialCholesky<SparseMatrix<double>> solver;
	solver.compute(A);

	VectorXd Vx = solver.solve(b_x);
	VectorXd Vy = solver.solve(b_y);
	VectorXd Vz = solver.solve(b_z);

	//set the solutions
	for (auto v : heMesh->Vertices())
	{
		int i = static_cast<int>(heMesh->Index(v));
		heMesh->Vertices()[i]->pos.at(0) = Vx(i);
		heMesh->Vertices()[i]->pos.at(1) = Vy(i);
		heMesh->Vertices()[i]->pos.at(2) = Vz(i);
	}
}
