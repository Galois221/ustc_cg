#include "ScanLine.h"
#include <iostream>

ScanLine::ScanLine()
{
}

ScanLine::~ScanLine()
{
}

int ScanLine::min(int a, int b)
{
	if (a < b) return a;
	return b;
}

vector<vector<int>> ScanLine::sorteg(vector<vector<int>> EG)
{
	
	for (int i = 0; i < EG.size(); i++)
	{
		if (!EG[i].empty())
		{   //Bubble Sort
			for (int j = 0; j < EG[i].size()-1; j++)
			{
				for (int k = 0; k < EG[i].size()-1 - j; j++)
				{
					if (EG[i][j] > EG[i][j + 1])
					{
						double tmp = EG[i][j + 1];
						EG[i][j + 1] = EG[i][j];
						EG[i][j] = tmp;
					}
				}
			}
		}
	}
	return EG;
}

MatrixXd ScanLine::Scan2Mat(int h, int w, vector<int> x_points, vector<int> y_points)
{
	MatrixXd selected = MatrixXd::Zero(h, w);
	vector<vector<Vertex>> V_EG(h, vector<Vertex>(0));
	
	for (int i = 0; i < y_points.size(); i++)
	{
		int front = i - 1;
		if (front < 0)front += y_points.size();
		
		if (y_points[front] < y_points[i])
		{
			Vertex tmp;
			tmp.x = x_points[front];
			tmp.dx = (x_points[i] - x_points[front]) / (double)(y_points[i] - y_points[front]);
			tmp.ymax = y_points[i];
			V_EG[y_points[front]].push_back(tmp);
		}
		if (y_points[front] > y_points[i])
		{
			Vertex tmp;
			tmp.x = x_points[i];
			tmp.dx = (x_points[i] - x_points[front]) / (double)(y_points[i] - y_points[front]);
			tmp.ymax = y_points[front];
			V_EG[y_points[i]].push_back(tmp);
		}
		if (y_points[front] == y_points[i])
		{
			int minmum = min(x_points[i], x_points[front]);
			int maxmum = x_points[i] + x_points[front] - minmum;
			for (int j = minmum; j < maxmum; j++)
				selected(y_points[i], j) = 0;
		}
	}

	//get EG from V_EG
	vector<vector<int>> EG(h, vector<int>(0));
	for (int i = 0; i < h; i++)
	{
		if (!V_EG[i].empty())
		{
			for (int j = 0; j < V_EG[i].size(); j++)
			{
				double rx = V_EG[i][j].x;
				for (int k = i; k < V_EG[i][j].ymax; k++) //exceptions excluded
				{
					EG[k].push_back((int)round(rx));
					rx += V_EG[i][j].dx;
				}
			}
		}
	}

	EG = sorteg(EG);

	//get region
	for (int i = 0; i < h; i++)
	{
		if (!EG[i].empty())
		{
			for (int k = 0; k < EG[i].size(); k += 2)
			{
				for (int j = EG[i][k] + 1; j < EG[i][k + 1]; j++)
				{
					selected(i, j) = 1;
				}
			}
		}
	}
	return selected;
}  