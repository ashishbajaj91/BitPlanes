#ifndef GETPARAMETERS_IS_INCLUDED
#define GETPARAMETERS_IS_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

void getWeights(int nrows, int ncols, double wts[8])
{
	wts[0] = 1.0;
	wts[1] = 1.0;
	wts[2] = 1.0204;
	wts[3] = 0.03125;
	wts[4] = 1.0313;
	wts[5] = 0.0204;
	wts[6] = 0.00055516;
	wts[7] = 0.00055516;

	double s = sqrt(double(nrows*ncols)) / 128.0;
	wts[2] = pow(wts[2],(1/s));
	wts[3] /= s;
	wts[4] = pow(wts[4], (1 / s));
	wts[5] /= s;
	wts[6] /= (s*s);
	wts[7] /= (s*s);
}

double getEpsilon()
{
	return double(1e-10);
}

double getLambda()
{
	return double(1e-6);
}

void getKeep(std::string method, int keep[8])
{
	for (int i = 0; i < 8; i++)
		keep[i] = 0;

	if (method == "translation")
	{
		keep[0] = 1;
		keep[1] = 1;
	}
	else if (method == "rigid")
	{
		keep[0] = 1;
		keep[1] = 1;
		keep[5] = 1;
	}
	else if (method == "similarity")
	{
		keep[0] = 1;
		keep[1] = 1;
		keep[2] = 1;
		keep[5] = 1;
	}
	else if (method == "affine")
	{
		keep[0] = 1;
		keep[1] = 1;
		keep[2] = 1;
		keep[3] = 1;
		keep[4] = 1;
		keep[5] = 1;
	}
	else if (method == "rotation")
	{
		keep[5] = 1;
		keep[6] = 1;
		keep[7] = 1;
	}
	else if (method == "projective")
	{
		keep[0] = 1;
		keep[1] = 1;
		keep[2] = 1;
		keep[3] = 1;
		keep[4] = 1;
		keep[5] = 1;
		keep[6] = 1;
		keep[7] = 1;
	}
	else
	{
		std::cout << "Invalid Method" << std::endl;
	}
}

int getNoOfParameters(bool keep[])
{
	int nParameters = 0;
	for (int i = 0; i < 8; i++)
		if (keep[i])
			nParameters++;
}

#endif