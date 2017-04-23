#ifndef GETPARAMETERS_IS_INCLUDED
#define GETPARAMETERS_IS_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

double * getWeights(int nrows, int ncols)
{
	double wts[] = { 1.0f, 1.0f, 1.0204f, 0.03125f, 1.0313f, 0.0204f, 0.00055516f, 0.00055516f };
	double s = sqrt(double(nrows*ncols)) / 128.0;
	wts[2] = pow(wts[2],(1/s));
	wts[3] /= s;
	wts[4] = pow(wts[4], (1 / s));
	wts[5] /= s;
	wts[6] /= (s*s);
	wts[7] /= (s*s);

	return wts;
}

double getEpsilon()
{
	return double(1e-3);
}

double getLambda()
{
	return double(1e-6);
}

double * getKeep(std::string method)
{
	double keep[] = {0.0, 0.0, 0.0, 0.0, 
					 0.0, 0.0, 0.0, 0.0};

	if (method == "transation")
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
	else if (method == "projectiive")
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
	return keep;
}

#endif