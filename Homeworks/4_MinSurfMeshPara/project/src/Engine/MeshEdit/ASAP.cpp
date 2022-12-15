#include <Engine/MeshEdit/ASAP.h>

#include <Engine/Primitive/TriMesh.h>

#include <Eigen/Sparse>

using namespace Ubpa;

using namespace std;
using namespace Eigen;
typedef Eigen::Triplet<double> T;

ASAP::ASAP(Ptr<TriMesh> triMesh)
	: heMesh(make_shared<HEMesh<V>>())
{
	Init(triMesh);
}

void ASAP::Clear() {
	heMesh->Clear();
	triMesh = nullptr;
}

bool ASAP::Init(Ptr<TriMesh> triMesh) {
	Clear();

	if (triMesh == nullptr)
		return true;

	if (triMesh->GetType() == TriMesh::INVALID) {
		printf("ERROR::ASAP::Init:\n"
			"\t""trimesh is invalid\n");
		return false;
	}

	//init half-edge structure
	size_t nV = triMesh->GetPositions().size();
	vector<vector<size_t>> triangles;
	triangles.reserve(triMesh->GetTriangles().size());
	for (auto triangle : triMesh->GetTriangles())
		triangles.push_back({ triangle->idx[0],triangle->idx[1], triangle->idx[2] });
	heMesh->Reserve(nV);
	heMesh->Init(triangles);

	if (!heMesh->IsTriMesh() || !heMesh->HaveBoundary()) {
		printf("ERROR::ASAP::Init:\n"
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

bool ASAP::Run() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::ASAP::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	ASAP_Para();

	//for (int i = 0; i < delta_X_y_list.size(); i++)
	//	cout << delta_X_y_list[i][0] << '\t' << delta_X_y_list[i][1] << '\t' << delta_X_y_list[i][2] << endl;
	//cout << delta_X_y_list.size() << endl;
	//for (int i = 0; i < cotweight_list.size(); i++) {
	//	for (int j = 0; j < cotweight_list[i].size(); j++) {
	//		cout << cotweight_list[i][j] << '\t';
	//	}
	//	cout<<endl;
	//}
	//cout << cotweight_list.size();

	//half-edge structure ->triangel mesh
	size_t nV = heMesh->NumVertices();
	size_t nF = heMesh->NumPolygons();
	vector<pointf3> positions;
	vector<unsigned> indice;
	positions.reserve(nV);
	indice.reserve(3 * nF);

	for (auto v : heMesh->Vertices())
		positions.push_back(v->coord.cast_to<pointf3>());
	for (auto f : heMesh->Polygons()) { // f is triangle
		for (auto v : f->BoundaryVertice()) // vertices of the triangle
			indice.push_back(static_cast<unsigned>(heMesh->Index(v)));
	}

	//cout << "ASAP done!" << endl;

	triMesh->Init(indice, positions);
	return true;
}

bool ASAP::Textcoord_Show() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::ASAP::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	ASAP_Para();

	//Update the textcoord 

	vector<pointf2> textcoord;

	for (auto v : heMesh->Vertices())
		textcoord.push_back(v->coord.cast_to<pointf2>());

	triMesh->Update(textcoord);
	return true;
}



void ASAP::ASAP_Para() {
	set_list();

	size_t nV = heMesh->NumVertices();
	size_t nT = heMesh->NumPolygons();
	vector<T> tripletlist;
	VectorXd b = VectorXd::Zero(2 * (size_t)nV + 2 * (size_t)nT);
	int id1 = heMesh->Index(heMesh->Boundaries()[0][0]->Origin());
	int id2 = heMesh->Index(heMesh->Boundaries()[0][(int)heMesh->Boundaries()[0].size() / 2]->End());

	// Build sparse matrix
	//1.Build coff of u_i (the qualtions between 1-2n)
	for (int i = 0; i < nV; i++)
	{
		double tp = 0;
		auto adjedges = heMesh->Vertices()[i]->OutHEs();

		if (i == id2) {
			tripletlist.push_back(T(2 * i, 2 * i, 1));
			tripletlist.push_back(T(2 * i + 1, 2 * i + 1, 1));
			b(2 * i) += 1;
			b(2 * i + 1) += 1;
			continue;
		}
		if (i == id1) {
			tripletlist.push_back(T(2 * i, 2 * i, 1));
			tripletlist.push_back(T(2 * i + 1, 2 * i + 1, 1));
			continue;
		}

		for (auto edge : adjedges) {
			double tmp = 0;
			int j = heMesh->Index(edge->End());
			if (edge->Polygon() != nullptr) {
				auto t = heMesh->Index(edge->Polygon());
				int i_id = 0;
				for (i_id = 0; heMesh->Index(edge->Polygon()->BoundaryVertice()[i_id]) != i; i_id++);
				tmp += cotweight_list[t][i_id == 0 ? 2 : i_id - 1];
			}
			if (edge->Pair()->Polygon() != nullptr) {
				auto t = heMesh->Index(edge->Pair()->Polygon());
				int i_id = 0;
				for (i_id = 0; heMesh->Index(edge->Pair()->Polygon()->BoundaryVertice()[i_id]) != i; i_id++);
				tmp += cotweight_list[t][i_id == 2 ? 0 : i_id + 1];
			}
			if (j != id1 && j!= id2) {
				tripletlist.push_back(T(2 * i, 2 * j, -tmp));
				tripletlist.push_back(T(2 * i + 1, 2 * j + 1, -tmp));
			}
			else if (j == id2) {
				b(2 * i) += (tmp);
				b(2 * i + 1) += (tmp);
			}
			tp += tmp;
		}
		tripletlist.push_back(T(2 * i, 2 * i, tp));
		tripletlist.push_back(T(2 * i + 1, 2 * i + 1, tp));
	}

	//2.Build coff of a_t,b_t (the qualtions between 1 to 2n)
	for (int i = 0; i < nV; i++)
	{
		auto adjedges = heMesh->Vertices()[i]->OutHEs();
		for (auto edge : adjedges) {
			auto triangle_l = edge->Polygon();
			auto triangle_r = edge->Pair()->Polygon();
			int j = heMesh->Index(edge->End());

			//left triangle
			if (triangle_l != nullptr) {
				size_t t_l = heMesh->Index(triangle_l);
				size_t id_l = 0;
				for (id_l = 0; heMesh->Index(triangle_l->BoundaryVertice()[id_l]) != i; id_l++);
				int id_lf = (id_l == 0) ? 2 : id_l - 1;
				//set cot_weight
				tripletlist.push_back(T(2 * i, 2 * nV + 2 * t_l, -cotweight_list[t_l][id_lf] * delta_X_x_list[t_l][id_l]));
				tripletlist.push_back(T(2 * i, 2 * nV + 2 * t_l + 1, -cotweight_list[t_l][id_lf] * delta_X_y_list[t_l][id_l]));
				tripletlist.push_back(T(2 * i + 1, 2 * nV + 2 * t_l, -cotweight_list[t_l][id_lf] * delta_X_y_list[t_l][id_l]));
				tripletlist.push_back(T(2 * i + 1, 2 * nV + 2 * t_l + 1, cotweight_list[t_l][id_lf] * delta_X_x_list[t_l][id_l]));
			}
			//right triangle
			if (triangle_r != nullptr) {
				size_t t_r = heMesh->Index(triangle_r);
				size_t id_r = 0;
				for (id_r = 0; heMesh->Index(triangle_r->BoundaryVertice()[id_r]) != j; id_r++);
				int id_rf = (id_r == 0) ? 2 : id_r - 1;
				//set cot_weight
				tripletlist.push_back(T(2 * i, 2 * nV + (size_t)2 * t_r, -cotweight_list[t_r][id_rf] * delta_X_x_list[t_r][id_r]));
				tripletlist.push_back(T(2 * i, 2 * nV + (size_t)2 * t_r + 1, -cotweight_list[t_r][id_rf] * delta_X_y_list[t_r][id_r]));
				tripletlist.push_back(T(2 * i + 1, 2 * nV + (size_t)2 * t_r, -cotweight_list[t_r][id_rf] * delta_X_y_list[t_r][id_r]));
				tripletlist.push_back(T(2 * i + 1, 2 * nV + (size_t)2 * t_r + 1, cotweight_list[t_r][id_rf] * delta_X_x_list[t_r][id_r]));
			}

		}
	}

	//3.Build qualtions between 2n+1 to 2n+2T
	for (int t = 0; t < nT; t++) {
		double c1 = 0;
		auto triangles = heMesh->Polygons();
		for (int i = 0; i < 3; i++) {
			int i_id = heMesh->Index(triangles[t]->BoundaryVertice()[i]);
			//int j_id = heMesh->Index(triangles[t]->BoundaryVertice()[i == 2 ? 0 : i + 1]);
			c1 += cotweight_list[t][i == 0 ? 2 : (i - 1)] * (delta_X_x_list[t][i] * delta_X_x_list[t][i] + delta_X_y_list[t][i] * delta_X_y_list[t][i]);
			if (i_id != id1 && i_id != id2) {
				//set C2
				tripletlist.push_back(T(2 * nV + (size_t)2 * t, 2 * i_id,
					-cotweight_list[t][i == 0 ? 2 : (i - 1)] * delta_X_x_list[t][i] + cotweight_list[t][i == 2 ? 0 : i + 1] * delta_X_x_list[t][i == 0 ? 2 : i - 1]));
				tripletlist.push_back(T(2 * nV + (size_t)2 * t, 2 * i_id + 1,
					-cotweight_list[t][i == 0 ? 2 : (i - 1)] * delta_X_y_list[t][i] + cotweight_list[t][i == 2 ? 0 : i + 1] * delta_X_y_list[t][i == 0 ? 2 : i - 1]));
				//set C3
				tripletlist.push_back(T(2 * nV + (size_t)2 * t+1, 2 * i_id,
					-cotweight_list[t][i == 0 ? 2 : (i - 1)] * delta_X_y_list[t][i] + cotweight_list[t][i == 2 ? 0 : i + 1] * delta_X_y_list[t][i == 0 ? 2 : i - 1]));
				tripletlist.push_back(T(2 * nV + (size_t)2 * t+1, 2 * i_id + 1,
					cotweight_list[t][i == 0 ? 2 : (i - 1)] * delta_X_x_list[t][i] - cotweight_list[t][i == 2 ? 0 : i + 1] * delta_X_x_list[t][i == 0 ? 2 : i - 1]));
			}
			if (i_id == id2) {
				b(2 * nV + 2 * t) += cotweight_list[t][i == 0 ? 2 : (i - 1)] * delta_X_x_list[t][i] - cotweight_list[t][i == 2 ? 0 : i + 1] * delta_X_x_list[t][i == 0 ? 2 : i - 1];
				b(2 * nV + 2 * t) += cotweight_list[t][i == 0 ? 2 : (i - 1)] * delta_X_y_list[t][i] - cotweight_list[t][i == 2 ? 0 : i + 1] * delta_X_y_list[t][i == 0 ? 2 : i - 1];
				b(2 * nV + 2 * t + 1) += cotweight_list[t][i == 0 ? 2 : (i - 1)] * delta_X_y_list[t][i] - cotweight_list[t][i == 2 ? 0 : i + 1] * delta_X_y_list[t][i == 0 ? 2 : i - 1];
				b(2 * nV + 2 * t+1) += -cotweight_list[t][i == 0 ? 2 : (i - 1)] * delta_X_x_list[t][i] + cotweight_list[t][i == 2 ? 0 : i + 1] * delta_X_x_list[t][i == 0 ? 2 : i - 1];
			}
		}

		//set C1
		tripletlist.push_back(T(2 * nV + (size_t)2 * t, 2 * nV + (size_t)2 * t, c1));
		tripletlist.push_back(T(2 * nV + (size_t)2 * t + 1, 2 * nV + (size_t)2 * t + 1, c1));
	}

	//cout << b << endl;

	//solve
	SparseMatrix<double>A(2 * nV + 2 * nT, 2 * nV + 2 * nT);
	A.setFromTriplets(tripletlist.begin(), tripletlist.end());
	A.makeCompressed();


	SimplicialCholesky<SparseMatrix<double>> solver;
	solver.compute(A);
	
	VectorXd X = solver.solve(b);
	for (auto v : heMesh->Vertices())
	{
		int i = static_cast<int>(heMesh->Index(v));
		heMesh->Vertices()[i]->pos.at(0) = X(2 * i);
		heMesh->Vertices()[i]->coord.at(0) = X(2 * i);
		heMesh->Vertices()[i]->pos.at(1) = X(2 * i + 1);
		heMesh->Vertices()[i]->coord.at(1) = X(2 * i + 1);
		heMesh->Vertices()[i]->pos.at(2) = 0;
	}
}

void ASAP::set_list()
{
	std::vector<std::vector<pointf2>> X_list;
	for (auto triangle : heMesh->Polygons())
	{
		vector<pointf2> tmp;
		auto v0 = triangle->BoundaryVertice()[0];
		auto v1 = triangle->BoundaryVertice()[1];
		auto v2 = triangle->BoundaryVertice()[2];
		double c_t0 = vecf3::cos_theta(v0->pos - v1->pos, v0->pos - v2->pos);
		double s_t0 = pow(1 - c_t0 * c_t0, 0.5);
		double e1 = (v0->pos - v1->pos).norm();
		double e2 = (v0->pos - v2->pos).norm();
		pointf2 t1 = { 0,0 };
		tmp.push_back(t1);
		pointf2 t2 = { e1,0 };
		tmp.push_back(t2);
		pointf2 t3 = { e2 * c_t0,e2 * s_t0 };
		tmp.push_back(t3);
		X_list.push_back(tmp);

		//set cotweight_list
		double c_t1 = vecf3::cos_theta(v1->pos - v0->pos, v1->pos - v2->pos);
		double s_t1 = pow(1 - c_t1 * c_t1, 0.5);
		double c_t2 = vecf3::cos_theta(v2->pos - v1->pos, v2->pos - v0->pos);
		double s_t2 = pow(1 - c_t2 * c_t2, 0.5);
		cotweight_list.push_back({ c_t0 / s_t0,c_t1 / s_t1,c_t2 / s_t2 });
	}
	for (auto j : X_list) {
		delta_X_x_list.push_back({ (double)j[0][0] - j[1][0],(double)j[1][0] - j[2][0],(double)j[2][0] - j[0][0] });
		delta_X_y_list.push_back({ (double)j[0][1] - j[1][1],(double)j[1][1] - j[2][1],(double)j[2][1] - j[0][1] });
	}
}

