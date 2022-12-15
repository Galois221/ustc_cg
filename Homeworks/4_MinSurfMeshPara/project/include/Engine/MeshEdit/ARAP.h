#pragma once

#include <Basic/HeapObj.h>
#include <UHEMesh/HEMesh.h>
#include <UGM/UGM>
#include <Eigen/Sparse>

namespace Ubpa {
	class TriMesh;
	class MinSurf;

	// mesh boundary == 1
	class ARAP : public HeapObj {
	public:
		ARAP(Ptr<TriMesh> triMesh);
	public:
		static const Ptr<ARAP> New(Ptr<TriMesh> triMesh) {
			return Ubpa::New<ARAP>(triMesh);
		}
	public:
		void Clear();
		bool Init(Ptr<TriMesh> triMesh);
		bool Run();
		bool Textcoord_Show();


	private:
		void ARAP_Para();
		void set_list();
		void Pre_decomposition();
		void Local();
		void Global();

	private:
		class V;
		class E;
		class P;
		class V : public TVertex<V, E, P> {
		public:
			vecf3 pos;
			vecf2 coord;
		};
		class E : public TEdge<V, E, P> { };
		class P : public TPolygon<V, E, P> { };

	private:
		Ptr<TriMesh> triMesh;
		const Ptr<HEMesh<V>> heMesh; // vertice order is same with triMesh
		std::vector<std::vector<double>> delta_X_x_list; //for each triangle
		std::vector<std::vector<double>> delta_X_y_list;
		std::vector<std::vector<vecf2>> x_list;
		std::vector<std::vector<double>> cotweight_list;
		std::vector<Eigen::Matrix2d> Lt_list;
		Eigen::SimplicialCholesky<Eigen::SparseMatrix<double>> solver;
		Eigen::VectorXd Ux;
		Eigen::VectorXd Uy;
	};
}
