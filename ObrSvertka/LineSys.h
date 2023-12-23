#pragma once
#include "Drawer.h"
#include <vector>
using namespace std;

class LineS
{
	int size;
	vector <double> lx, lh, ly, SearchX;

public:
	LineS(double* A, double* stok, double* mat, int N, double fd);
	void GetX(vector <double>& px);
	void GetH(vector <double>& ph);
	void CreateY(double fd);
	void GetY(vector<double>& py);
	int Get_size();
	double function(float* lyambda);
	void MHJ(float* lyambda, float& h, float TAU, double fd, int N, MSG& msg, bool flag, Drawer& d);
	void DekonvSvertka(float* lyambda);
	void GetSearchX(vector<double>& sx);
};

void H_for_gr(vector<double>& ph);
double E(vector<double> x);