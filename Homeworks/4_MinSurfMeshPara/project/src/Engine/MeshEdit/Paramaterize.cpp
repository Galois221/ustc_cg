#include <Engine/MeshEdit/Paramaterize.h>

#include <Engine/MeshEdit/MinSurf.h>

#include <Engine/Primitive/TriMesh.h>

#include <Eigen/Sparse>

using namespace Ubpa;

using namespace std;
using namespace Eigen;
typedef Eigen::Triplet<double> T;

Paramaterize::Paramaterize(Ptr<TriMesh> triMesh)
	: heMesh(make_shared<HEMesh<V>>())
{
	Init(triMesh);
}

void Paramaterize::Clear() {
	heMesh->Clear();
	triMesh = nullptr;
}

bool Paramaterize::Init(Ptr<TriMesh> triMesh) {
	Clear();

	if (triMesh == nullptr)
		return true;

	if (triMesh->GetType() == TriMesh::INVALID) {
		printf("ERROR::Paramaterize::Init:\n"
			"\t""trimesh is invalid\n");
		return false;
	}

	//init half-edge structure
	size_t nV = triMesh->GetPositions().size();
	vector<vector<size_t>> triangles;
	triangles.reserve(triMesh->GetTriangles().size());
	for (auto triangle : triMesh->GetTriangles())
		triangles.push_back({ triangle->idx[0],triangle->idx[1], triangle->idx[2]});
	heMesh->Reserve(nV);
	heMesh->Init(triangles);

	if (!heMesh->IsTriMesh() || !heMesh->HaveBoundary()) {
		printf("ERROR::Paramaterize::Init:\n"
			"\t""trimesh is not a triangle mesh or hasn't a boundaries\n");
		heMesh->Clear();
		return false;
	}

	// triangle mesh's positions ->  half-edge structure's positions
	for (int i = 0; i < nV; i++) {
		auto v = heMesh->Vertices().at(i);
		v->coord = triMesh->GetPositions()[i].cast_to<vecf2>();
		v->pos = triMesh->GetPositions()[i].cast_to<vecf3>();
	}

	this->triMesh = triMesh;
	return true;
}

bool Paramaterize::Run() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::Paramaterize::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}
	
	para();
	
	//half-edge structure ->triangel mesh
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

bool Paramaterize::Textcoord_Show() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::Paramaterize::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	para();

	//Update the textcoord 

	vector<pointf2> textcoord;

	for (auto v : heMesh->Vertices())
		textcoord.push_back(v->coord.cast_to<pointf2>());

	triMesh->Update(textcoord);
	return true;
}


void Paramaterize::Set_Boundary() {
	auto bd = heMesh->Boundaries()[0];
	float t = 0;
	float dt = 360 / static_cast<float>(bd.size());

	if (shape == 0) {
		t = -45;
		for (auto he : bd) {
			auto v = he->Origin();
			if (t >= -45 && t < 45) {
				pointf2 tmp = { 1,0.5 * tan(t * PI<double> / 180.) + 0.5 };
				v->coord = tmp.cast_to<vecf2>();
			}
			if (t >= 45 && t < 135) {
				pointf2 tmp = { 0.5 / tan(t * PI<double> / 180.) + 0.5,1 };
				v->coord = tmp.cast_to<vecf2>();
			}
			if (t >= 135 && t < 225) {
				pointf2 tmp = { 0,-0.5 * tan(t * PI<double> / 180.) + 0.5 };
				v->coord = tmp.cast_to<vecf2>();
			}
			if (t >= 225 && t < 315) {
				pointf2 tmp = { -0.5 / tan(t * PI<double> / 180.) + 0.5,0 };
				v->coord = tmp.cast_to<vecf2>();
			}
			t += dt;
		}
	}
	else if (shape == 1) {
		for (auto he : bd) {
			auto v = he->Origin();
			pointf2 tmp = { 0.5 * sin(t * PI<double> / 180.) + 0.5,0.5 * cos(t * PI<double> / 180.) + 0.5 };
			v->coord = tmp.cast_to<vecf2>();
			t += dt;
		}
	}
}

void Paramaterize::para() {
	Set_Boundary();

	size_t nV = heMesh->NumVertices();
	vector<T> tripletlist;
	VectorXd b_x = VectorXd::Zero(nV);
	VectorXd b_y = VectorXd::Zero(nV);

	for (auto v : heMesh->Vertices()) {
		int i = static_cast<int>(heMesh->Index(v));
		// if v_i is in the boundary
		if (v->IsBoundary())
		{

			b_x(i) += v->coord[0];
			b_y(i) += v->coord[1];
			tripletlist.push_back(T(i, i, 1));
			continue;
		}
		if (!v->IsBoundary()) {
			if (weight == 0) {
				//if v_i is not in boundary
				tripletlist.push_back(T(i, i, v->Degree()));
				for (auto n : v->AdjVertices()) {
					int j = static_cast<int>(heMesh->Index(n));
					if (n->IsBoundary()) {
						b_x(i) += n->coord[0];
						b_y(i) += n->coord[1];
					}
					else
					{
						tripletlist.push_back(T(i, j, -1));
					}
				}
			}
			else if (weight == 1)
			{
				vector<double> weight_list;
				w_cotangent(v, weight_list);
				double sum = 0;
				for (size_t i = 0; i < weight_list.size(); i++)
				{
					sum += weight_list[i];
				}
				tripletlist.push_back(T(i, i, sum));
				for (size_t j = 0;j<v->AdjVertices().size();j++) {
					V* n = v->AdjVertices()[j];
					if (n->IsBoundary()) {
						b_x(i) += (n->coord[0]) * weight_list[j];
						b_y(i) += (n->coord[1]) * weight_list[j];
					}
					else
					{
						tripletlist.push_back(T(i, static_cast<int>(heMesh->Index(n)), -weight_list[j]));
					}
				}
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

	//set the solutions
	for (auto v : heMesh->Vertices())
	{
		int i = static_cast<int>(heMesh->Index(v));
		heMesh->Vertices()[i]->pos.at(0) = Vx(i);
		heMesh->Vertices()[i]->coord.at(0) = Vx(i);
		heMesh->Vertices()[i]->pos.at(1) = Vy(i);
		heMesh->Vertices()[i]->coord.at(1) = Vy(i);
		heMesh->Vertices()[i]->pos.at(2) = 0;
	}
}

void Paramaterize::Set_Shape(int a)
{
	shape = a;
}

void Paramaterize::Set_Weight(int a)
{
	weight = a;
}

void Paramaterize::w_cotangent(V* v, std::vector<double> &weight_list)
{
	auto adjecent_list = v->AdjVertices();
	for (int i = 0; i < adjecent_list.size(); i++) {
		int front = (i - 1) >= 0 ? (i - 1) : adjecent_list.size() - 1;
		int next = i < (adjecent_list.size() - 1) ? (i + 1) : 0;
		V* v0 = v->AdjVertices()[i];
		V* v1 = v->AdjVertices()[front];
		V* v2 = v->AdjVertices()[next];
		double s_t,c_t,s_b,c_b;
		c_t = vecf3::cos_theta(v1->pos - v0->pos, v1->pos - v->pos);
		c_b = vecf3::cos_theta(v2->pos - v0->pos, v2->pos - v->pos);
		s_t = pow(1 - c_t * c_t, 0.5);
		s_b = pow(1 - c_b * c_b, 0.5);
		weight_list.push_back(c_t / s_t + c_b / s_b);
	}
}
