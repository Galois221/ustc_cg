#pragma once

#include <Basic/HeapObj.h>
//#include <Engine/Primitive/MassSpring.h>
#include <UGM/UGM>
#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace Ubpa {
	class Simulate : public HeapObj {
	public:
		Simulate(const std::vector<pointf3>& plist,
			const std::vector<unsigned>& elist) {
			edgelist = elist;
			this->positions.resize(plist.size());
			for (int i = 0; i < plist.size(); i++)
			{
				for (int j = 0; j < 3; j++)
				{
					this->positions[i][j] = plist[i][j];
				}
			}
		};
	public:
		static const Ptr<Simulate> New(const std::vector<pointf3>& plist,
			const std::vector<unsigned> &elist) {
			return Ubpa::New<Simulate>(plist, elist);
		}
	public:
		// clear cache data
		void Clear();

		// init cache data (eg. half-edge structure) for Run()
		bool Init();
		//bool Init();

		// call it after Init()
		bool Run();
		
		const std::vector<pointf3>& GetPositions() const { return positions; };

		const float GetStiff() { return stiff; };
		void SetStiff(float k) { stiff = k; Init();};
		const float GetTimeStep() { return h; };
		void SetTimeStep(float k) { h = k; Init();};
		std::vector<unsigned>& GetFix() { return this->fixed_id; };
		void SetFix(const std::vector<unsigned>& f) { this->fixed_id = f; Init();};
		const std::vector<pointf3>& GetVelocity() { return velocity; };
		//void SetVelocity(const std::vector<pointf3>& v) { velocity = v; };
		void SetEuler_Method() { this->method = 1; Init(); };
		void SetFast_Method() { this->method = 2; Init(); };
		void SetLeftFix();


	private:
		// kernel part of the algorithm
		void EulerImplicit_Method();
		void Fast_Method();
		void GetInit_length();
		void SetCoffMatrix();

	private:
		float h = 0.003f;  //²½³¤
		float stiff=100000;
		float M = 1.0f;
		int method = 1;
		std::vector<unsigned> fixed_id;  //fixed point id
		std::vector<bool> is_fixed;

		//mesh data
		std::vector<unsigned> edgelist;


		//simulation data
		std::vector<pointf3> positions;
		std::vector<pointf3> velocity;

		std::vector<double> init_length;
		Eigen::SparseMatrix<double> K;
		Eigen::VectorXd b;//X-K*K^T*X

		//Euler Method
		Eigen::VectorXd f_ext;
		Eigen::VectorXd f_int;
		Eigen::SparseMatrix<double> Coff;
		Eigen::SparseMatrix<double> J;
		Eigen::SparseLU<Eigen::SparseMatrix<double>> Coff_LU;
		Eigen::SparseMatrix<double> Mh2L;
	};
}
