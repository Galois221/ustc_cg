#include <Engine/MeshEdit/ARAP.h>
#include <Engine/MeshEdit/ASAP.h>
#include <Engine/MeshEdit/Paramaterize.h>

#include <Engine/Primitive/TriMesh.h>

#include <Eigen/Sparse>
#include <Eigen/Dense>

using namespace Ubpa;

using namespace std;

using namespace Eigen;
typedef Eigen::Triplet<double> T;

ARAP::ARAP(Ptr<TriMesh> triMesh)
	: heMesh(make_shared<HEMesh<V>>())
{
	Init(triMesh);
}

void ARAP::Clear() {
	heMesh->Clear();
	triMesh = nullptr;
}

bool ARAP::Init(Ptr<TriMesh> triMesh) {
	Clear();

	if (triMesh == nullptr)
		return true;

	if (triMesh->GetType() == TriMesh::INVALID) {
		printf("ERROR::ARAP::Init:\n"
			"\t""trimesh is invalid\n");
		return false;
	}
	
	auto asap = ASAP::New(triMesh);
	asap->Textcoord_Show();
	/*
	auto paramaterize = Paramaterize::New(triMesh);
	paramaterize->Set_Shape(1);
	paramaterize->Set_Weight(0);
	paramaterize->Run(); */

	//init half-edge structure
	size_t nV = triMesh->GetPositions().size();
	vector<vector<size_t>> triangles;
	triangles.reserve(triMesh->GetTriangles().size());
	for (auto triangle : triMesh->GetTriangles())
		triangles.push_back({ triangle->idx[0],triangle->idx[1], triangle->idx[2] });
	heMesh->Reserve(nV);
	heMesh->Init(triangles);

	if (!heMesh->IsTriMesh() || !heMesh->HaveBoundary()) {
		printf("ERROR::ARAP::Init:\n"
			"\t""trimesh is not a triangle mesh or hasn't a boundaries\n");
		heMesh->Clear();
		return false;
	}

	// triangle mesh's positions ->  half-edge structure's positions
	for (int i = 0; i < nV; i++) {
		auto v = heMesh->Vertices().at(i);
		v->coord = triMesh->GetTexcoords()[i].cast_to<vecf2>();
		v->pos = triMesh->GetPositions()[i].cast_to<vecf3>();
	}

	set_list();

	this->triMesh = triMesh;

	return true;
}

bool ARAP::Run() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::ARAP::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}
	//set_list();
	//Pre_decomposition();
	
	ARAP_Para();

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

	triMesh->Init(indice, positions);
	return true;
}

bool ARAP::Textcoord_Show() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::ARAP::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	ARAP_Para();

	//Update the textcoord 

	vector<pointf2> textcoord;

	for (auto v : heMesh->Vertices())
		textcoord.push_back(v->coord.cast_to<pointf2>());

	triMesh->Update(textcoord);
	return true;
}


void ARAP::ARAP_Para() {
	//set_list();
	Pre_decomposition();

	int t = 0;
	int times = 1;
	while (t < times)
	{
		Local();
		Global();
		t++;
	}
	for (int i = 0; i < heMesh->NumVertices(); i++) {
		heMesh->Vertices()[i]->pos[0] = Ux(i);
		heMesh->Vertices()[i]->pos[1] = Uy(i);
		heMesh->Vertices()[i]->pos[2] = 0;
	}
}

void ARAP::set_list()
{
	std::vector<std::vector<pointf2>> X_list;
	size_t nT = heMesh->NumPolygons();
	auto triangles = heMesh->Polygons();
	for (int t = 0; t < nT; t++)
	{
		auto triangle = triangles[t];
		vector<pointf2> tmp;
		vector<vecf2> tp;
		auto v0 = triangle->BoundaryVertice()[0];
		auto v1 = triangle->BoundaryVertice()[1];
		auto v2 = triangle->BoundaryVertice()[2];
		double c_t0 = vecf3::cos_theta(v0->pos - v1->pos, v0->pos - v2->pos);
		double s_t0 = pow(1 - c_t0 * c_t0, 0.5);
		double e1 = (v0->pos - v1->pos).norm();
		double e2 = (v0->pos - v2->pos).norm();
		pointf2 t1 = { 0,0 };
		tmp.push_back(t1);
		tp.push_back(vecf2(0, 0));
		pointf2 t2 = { e1,0 };
		tmp.push_back(t2);
		tp.push_back(vecf2(e1, 0));
		pointf2 t3 = { e2 * c_t0,e2 * s_t0 };
		tmp.push_back(t3);
		tp.push_back(vecf2(e2 * c_t0, e2 * s_t0));
		X_list.push_back(tmp);
		x_list.push_back(tp);
		
		//set cotweight_list
		double c_t1 = vecf3::cos_theta(v1->pos - v2->pos, v1->pos - v0->pos);
		double s_t1 = pow(1 - c_t1 * c_t1, 0.5);
		double c_t2 = vecf3::cos_theta(v2->pos - v0->pos, v2->pos - v1->pos);
		double s_t2 = pow(1 - c_t2 * c_t2, 0.5);
		cotweight_list.push_back({ c_t0 / s_t0,c_t1 / s_t1,c_t2 / s_t2 });
	}
	//for (auto a : X_list)cout << a[0].cast_to<vecf2>() << a[1].cast_to<vecf2>() << a[2].cast_to<vecf2>()<<endl;

	//set delta_x_list
	for (auto j : X_list) {
		delta_X_x_list.push_back({ (double)j[0][0] - j[1][0],(double)j[1][0] - j[2][0],(double)j[2][0] - j[0][0] });
		delta_X_y_list.push_back({ (double)j[0][1] - j[1][1],(double)j[1][1] - j[2][1],(double)j[2][1] - j[0][1] });
	}
}

void ARAP::Pre_decomposition() {
	size_t nV = heMesh->NumVertices();
	vector<T> tripletlist;
	size_t id1 = heMesh->Index(heMesh->Boundaries()[0][0]->Origin());

	for (size_t i = 0; i < nV; i++)
	{
		double tp = 0;
		auto adjedges = heMesh->Vertices()[i]->OutHEs();

		if (i == id1) {
			tripletlist.push_back(T(i, i, 1));
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
			if (j != id1) {
				tripletlist.push_back(T(i, j, -tmp));
			}
			tp += tmp;
		}
		tripletlist.push_back(T(i, i, tp));
	}
	SparseMatrix<double>A(nV, nV);
	A.setFromTriplets(tripletlist.begin(), tripletlist.end());
	//cout << A;
	A.makeCompressed();
	solver.compute(A);
}

void ARAP::Local() {
	Lt_list.clear();
	
	for (auto triangle : heMesh->Polygons()) {
		Matrix2d S_t = Matrix2d::Zero(2, 2);
		auto vertices = triangle->BoundaryVertice();
		size_t t = heMesh->Index(triangle);
		for (int i = 0; i < 3; i++) {
			Matrix2d tmp;
			Vector2d u_t, x_t;
			u_t = { (vertices[i]->coord - vertices[i == 2 ? 0 : i + 1]->coord)[0],(vertices[i]->coord - vertices[i == 2 ? 0 : i + 1]->coord)[1] };
			x_t = { delta_X_x_list[t][i],delta_X_y_list[t][i] };
			tmp = cotweight_list[t][i == 0 ? 2 : i - 1] * u_t * (x_t.transpose());
			S_t += tmp;
		}
		JacobiSVD<MatrixXd> svd(S_t, ComputeFullU | ComputeFullV);
		Matrix2d U_t = svd.matrixU();
		Matrix2d V_t = svd.matrixV();
		Matrix2d L_t = U_t * (V_t.transpose());
		//update L_t
		if (L_t.determinant() > 0) {
			Lt_list.push_back(L_t);
			//cout << "L:" << L_t << endl;
		}
		else {
			Matrix2d fh;
			fh << 1, 0,
				0, -1;
			Lt_list.push_back(U_t * fh * V_t.transpose());
			//cout << "L:" << U_t * fh * V_t.transpose() << endl;
		}
		
		/*Matrix2d X, U;
		auto vertices = triangle->BoundaryVertice();
		size_t t = heMesh->Index(triangle);
		//X << delta_X_x_list[t][0], delta_X_x_list[t][1],
		//	delta_X_y_list[t][0], delta_X_y_list[t][1];
		X << (x_list[t][0] - x_list[t][1])[0], (x_list[t][1] - x_list[t][2])[0],
			(x_list[t][0] - x_list[t][1])[1], (x_list[t][1] - x_list[t][2])[1];
		U << (vertices[0]->coord - vertices[1]->coord)[0], (vertices[1]->coord - vertices[2]->coord)[0],
			(vertices[0]->coord - vertices[1]->coord)[1], (vertices[1]->coord - vertices[2]->coord)[1];
		//cout << "U:" << U << endl;
		cout << "X:" << X << endl;
		Matrix2d S_t = U * X.inverse();
		JacobiSVD<MatrixXd> svd(S_t, ComputeFullU | ComputeFullV);
		if (S_t.determinant() > 0) {
			Lt_list.push_back(svd.matrixU() * svd.matrixV().transpose());
			//cout << "L:" << svd.matrixU() * svd.matrixV().transpose() << endl;
		}
		else {
			Matrix2d D;
			D(0, 0) = 1; D(0, 1) = 0; D(1, 0) = 0; D(1, 1) = -1;
			Lt_list.push_back(svd.matrixU() *D* svd.matrixV().transpose());
			//cout << "L:" << svd.matrixU() * D * svd.matrixV().transpose() << endl;
		}*/
	}
}

void ARAP::Global() {
	size_t nV = heMesh->NumVertices();
	size_t id1 = heMesh->Index(heMesh->Boundaries()[0][0]->Origin());
	VectorXd bx = VectorXd::Zero(nV);
	VectorXd by = VectorXd::Zero(nV); //u_x and u_y are independent

	for (int i = 0; i < nV; i++)
	{
		if (i == id1) {
			bx(i) = by(i) = 0;
			continue;
		}
		auto adjedges = heMesh->Vertices()[i]->OutHEs();
		Vector2d sum = Vector2d::Zero();
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
				//sum += cotweight_list[t_l][id_lf] * Lt_list[t_l] * Vector2d(delta_X_x_list[t_l][id_l], delta_X_y_list[t_l][id_l]);
				bx(i) += (cotweight_list[t_l][id_lf] * delta_X_x_list[t_l][id_l] * (Lt_list[t_l](0, 0))
					+ cotweight_list[t_l][id_lf] * delta_X_y_list[t_l][id_l] * (Lt_list[t_l](0, 1)));
				by(i) += (cotweight_list[t_l][id_lf] * delta_X_x_list[t_l][id_l] * (Lt_list[t_l](1, 0))
					+ cotweight_list[t_l][id_lf] * delta_X_y_list[t_l][id_l] * (Lt_list[t_l](1, 1)));
			}
			//right triangle
			if (triangle_r != nullptr) {
				size_t t_r = heMesh->Index(triangle_r);
				size_t id_r = 0;
				for (id_r = 0; heMesh->Index(triangle_r->BoundaryVertice()[id_r]) != j; id_r++);
				int id_rf = (id_r == 0) ? 2 : id_r - 1;
				//set cot_weight
				//sum -= cotweight_list[t_r][id_rf] * Lt_list[t_r] * Vector2d(delta_X_x_list[t_r][id_r], delta_X_y_list[t_r][id_r]);
				bx(i) -= (cotweight_list[t_r][id_rf] * delta_X_x_list[t_r][id_r] * (Lt_list[t_r](0, 0))
					+ cotweight_list[t_r][id_rf] * delta_X_y_list[t_r][id_r] * (Lt_list[t_r](0, 1)));
				by(i) -= (cotweight_list[t_r][id_rf] * delta_X_x_list[t_r][id_r] * (Lt_list[t_r](1, 0))
					+ cotweight_list[t_r][id_rf] * delta_X_y_list[t_r][id_r] * (Lt_list[t_r](1, 1)));
			}
		}
		//bx(i) += sum(0);
		//by(i) += sum(1);
	}

	Ux = solver.solve(bx);
	Uy = solver.solve(by);
	//update u_list;
	for (int i = 0; i < nV; i++)
	{
		heMesh->Vertices()[i]->coord = vecf2(Ux(i), Uy(i));
	}
	//cout << Ux(1)<<'\t' << Uy(1) << endl;
	//cout << Ux(id1) << '\t' << Uy(id1) << endl;
	//cout << "size:" << u_list.size() << endl;
	//cout << endl;
}