#include "pch.h"
#define _USE_MATH_DEFINES
#include "LineSys.h"
#include <math.h>

LineS::LineS(double* A, double* stok, double* mat, int N, double fd)
{
	size = N;
	for (int i = 0; i < N; i++)
	{
		lx.push_back(0.);
		lh.push_back(0.);
		for (int j = 0; j < 3; j++)
		{
			lx[i] += A[j] * exp(-((double)i / fd - mat[j]) * ((double)i / fd - mat[j]) / (stok[j] * stok[j]));
		}

		if (i < N / 2) lh[i] += A[3] * exp(-((double)(i) / fd) * ((double)(i) / fd) / (stok[3] * stok[3]));
		else lh[i] = A[3] * exp(-((double)(i - N) / fd) * ((double)(i - N) / fd) / (stok[3] * stok[3]));
	}
	lh.push_back(A[3]);
}

void LineS::GetX(vector<double>& px)
{
	px.clear();
	px = lx;
}

void LineS::GetH(vector<double>& ph)
{
	ph.clear();
	ph = lh;
}

void LineS::CreateY(double fd)
{
	for (int i = 0; i < size; i++)
	{
		ly.push_back(0.);
		for (int k = 0; k < size; k++)
		{
			ly[i] += lx[k] * lh[abs(i - k)];
		}
		//ly[i] /= size;
		//ly[i] *= fd;
	}
}

void LineS::GetY(vector<double>& py)
{
	py.clear();
	py = ly;
}

int LineS::Get_size()
{
	return size;
}

double LineS::function(float* l)
{
	float* work = new float[size];
	float sum_k, po = 0.;
	for (int k = 0; k < size; k++)
	{
		sum_k = 0.;
		for (int j = 0; j < size; j++)
		{
			sum_k += lh[abs(j - k)] * l[j];
		}
		work[k] = exp(-1 - sum_k);
	}

	for (int i = 0; i < size; i++)
	{
		sum_k = 0;
		for (int k = 0; k < size; k++)
		{
			sum_k += lh[abs(i - k)] * work[k];
		}
		po += (ly[i] - sum_k) * (ly[i] - sum_k);
	}

	delete[] work;
	return po;
}

void LineS::MHJ(float* x, float& h, float TAU, double fd, int N, MSG& msg, bool flag, Drawer& d)
{
	// kk - количество параметров; x - массив параметров
	//float  TAU = 1.e-6f; // Точность вычислений
	int i, j, bs, ps, kk = size, calc = 0;
	float z, k, fi, fb;
	float* b = new float[kk];
	float* y = new float[kk];
	float* p = new float[kk];

	//h = 1.;
	if (!flag)
	{
		x[0] = 1.;
		for (i = 1; i < kk; i++)  x[i] = (float)rand() / RAND_MAX * 2 - 1; // Задается начальное приближение
	}

	k = h;
	for (i = 0; i < kk; i++)	y[i] = p[i] = b[i] = x[i];
	fi = function(x);
	ps = 0;   bs = 1;  fb = fi;

	j = 0;
	while (1)
	{
		calc++; // Счетчик итераций. Можно игнорировать

		if (calc % 100 == 0)
		{
			DekonvSvertka(x);
			d.DrawTwoSig(lx, SearchX, L"t", L"A", N / fd, 1 / fd);
		}

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.lParam == (LPARAM("PAUSE")))
			{
				h = k;
				return;
			}
		}

		x[j] = y[j] + k;
		z = function(x);
		if (z >= fi) {
			x[j] = y[j] - k;
			z = function(x);
			if (z < fi)   y[j] = x[j];
			else  x[j] = y[j];
		}
		else  y[j] = x[j];
		fi = function(x);

		if (j < kk - 1) { j++;  continue; }
		if (fi + 1e-8 >= fb) {
			if (ps == 1 && bs == 0) {
				for (i = 0; i < kk; i++) {
					p[i] = y[i] = x[i] = b[i];
				}
				z = function(x);
				bs = 1;   ps = 0;   fi = z;   fb = z;   j = 0;
				continue;
			}
			k /= 10.;
			if (k < TAU) break;
			j = 0;
			continue;
		}

		for (i = 0; i < kk; i++) {
			p[i] = 2 * y[i] - b[i];
			b[i] = y[i];
			x[i] = p[i];

			y[i] = x[i];
		}
		z = function(x);
		fb = fi;   ps = 1;   bs = 0;   fi = z;   j = 0;
	} //  end of while(1)

	for (i = 0; i < kk; i++)  x[i] = p[i];

	delete[] b;
	delete[] y;
	delete[] p;
}

void LineS::DekonvSvertka(float* l)
{
	if (SearchX.size() != 0) SearchX.clear();
	double sum_k = 0.;
	for (int j = 0; j < size; j++)
	{
		for (int k = 0; k < size; k++)
		{
			sum_k += lh[abs(k - j)] * l[k];
		}
		SearchX.push_back(exp(-1 - sum_k));
		sum_k = 0.;
	}
}

void LineS::GetSearchX(vector<double>& sx)
{
	sx.clear();
	sx = SearchX;
}

void H_for_gr(vector<double>& ph)
{
	vector<double> ::iterator it = ph.begin();
	it += ph.size() / 2;
	vector<double> p1(ph.begin(), it);
	vector<double> p2(it, ph.end());
	ph.clear();
	ph.insert(ph.end(), p2.begin(), p2.end());
	ph.insert(ph.end(), p1.begin(), p1.end());
}

double E(vector<double> x)
{
	double energy(0.);

	for (int i = 0; i < x.size(); i++)
	{
		energy += x[i] * x[i];
	}

	return energy;
}
