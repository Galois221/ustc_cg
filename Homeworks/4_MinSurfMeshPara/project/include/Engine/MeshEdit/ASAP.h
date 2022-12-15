#pragma once

#include <Basic/HeapObj.h>
#include <UHEMesh/HEMesh.h>
#include <UGM/UGM>

namespace Ubpa {
	class TriMesh;
	class MinSurf;

	// mesh boundary == 1
	class ASAP : public HeapObj {
	public:
		ASAP(Ptr<TriMesh> triMesh);
	public:
		static const Ptr<ASAP> New(Ptr<TriMesh> triMesh) {
			return Ubpa::New<ASAP>(triMesh);
		}
	public:
		void Clear();
		bool Init(Ptr<TriMesh> triMesh);
		bool Run();
		bool Textcoord_Show();
		void set_list();

	private:
		void ASAP_Para();
		

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
		std::vector<std::vector<double>> delta_X_x_list;
		std::vector<std::vector<double>> delta_X_y_list;
		std::vector<std::vector<double>> cotweight_list;
		std::vector<std::vector<pointf2>> u_list;
	};
}
