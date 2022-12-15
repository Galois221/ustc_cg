#pragma once

#include <Basic/HeapObj.h>
#include <UHEMesh/HEMesh.h>
#include <UGM/UGM>

namespace Ubpa {
	class TriMesh;
	class MinSurf;

	// mesh boundary == 1
	class Paramaterize : public HeapObj {
	public:
		Paramaterize(Ptr<TriMesh> triMesh);
	public:
		static const Ptr<Paramaterize> New(Ptr<TriMesh> triMesh) {
			return Ubpa::New<Paramaterize>(triMesh);
		}
	public:
		void Clear();
		bool Init(Ptr<TriMesh> triMesh);
		bool Run();
		bool Textcoord_Show();
		void Set_Shape(int a);
		void Set_Weight(int a);
	private:
		void Set_Boundary();
		void para();

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
		class P : public TPolygon<V,E,P> { };
		
	private:
		Ptr<TriMesh> triMesh;
		const Ptr<HEMesh<V>> heMesh; // vertice order is same with triMesh
		int shape = 0;
		int weight = 0;
		void w_cotangent(V* v, std::vector<double> &weight_list);
	};
}
