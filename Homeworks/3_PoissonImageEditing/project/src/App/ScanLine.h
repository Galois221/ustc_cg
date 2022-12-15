#pragma once
#include <Eigen/Dense>
#include <vector>
using namespace Eigen;
using namespace std;

struct Vertex
{
	double x;  //record the x_value of the lower vertex,notice this is double type
	double dx;
	int ymax;
};

class ScanLine {
public:
	ScanLine();
	~ScanLine();

	MatrixXd Scan2Mat(int h, int w, vector<int>x_points, vector<int>y_points);

private:
	int min(int a, int b);
	vector<vector<int>> sorteg(vector<vector<int>> EG);
};