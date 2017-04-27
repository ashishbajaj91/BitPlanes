#ifndef LUKASKANADE_IS_INCLUDED
#define LUKASKANADE_IS_INCLUDED

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "getParameters.h"
#include "matrixFunctions.h"

double * Initializeds()
{
	double ds[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	return ds;
}

double computedsForOperation(cv::Mat &M, cv::Mat &Ds, cv::Mat &dI)
{

}

double * Computeds(std::vector<cv::Mat> Ds, cv::Mat &M, cv::Mat &dI)
{
	for (int i = 0; i < 8; i++)
	{
		cv::Mat_<double> D;
		cv::multiply(Ds[i], M, D);

		cv::Mat_<double> temp;
		cv::multiply(dI, M, temp);

		cv::Mat_<double> temp2;
		cv::multiply(D, temp, temp2);
		auto va1 = cv::sum(temp2);

		cv::Mat_<double> temp3;
		cv::multiply(D, D, temp3);
		auto val2 = cv::sum(temp3);

		//auto arr = 1 / (val2 + lambda)*temp;

	}




}

cv::Mat ComputeError(std::vector<cv::Mat> &Ip, std::vector<cv::Mat> &Iref)
{
	cv::Mat_<double> dI;
	auto errorPlanes = ComputeAbsoluteDifferenceBitPlanes(Ip, Iref);
	for (int i = 0; i < errorPlanes.size(); i++)
	{
		if (i == 0)
			dI = errorPlanes[i];
		else
			dI = dI + errorPlanes[i];
	}
	return dI;
}

cv::Mat ComputeSumedSubtraction(std::vector<cv::Mat> &Ip, std::vector<cv::Mat> &Iref)
{
	cv::Mat_<double> dI;
	auto errorPlanes = SubtractBitPlanes(Ip, Iref);
	for (int i = 0; i < errorPlanes.size(); i++)
	{
		if (i == 0)
			dI = errorPlanes[i];
		else
			dI = dI + errorPlanes[i];
	}
	return dI;
}

bool LukasKanade(std::vector<cv::Mat> &I, std::vector<cv::Mat> &Iref, Eigen::Matrix3d &H,
				 std::vector<cv::Mat> Ds, cv::Mat Mref, 
				 double *wts, double *keep, double epsilon, double lambdathreshold)
{
	double error = std::numeric_limits<double>::infinity();
	double * ds = Initializeds();

	double h = I[0].rows - 2;
	double w = I[0].cols - 2;

	cv::Mat_<double> lambda = lambdathreshold*cv::Mat_<double>::eye(8,8);

	for (int i = 0; i < 100; ++i)
	{

		auto Ip = ApplyWarpToBitPlanes(I, H);
		auto dI = ComputeSumedSubtraction(Ip, Iref);
		auto dI0 = ComputeError(Ip, Iref);

		cv::Mat_<double> M = cv::Mat(Mref & ~CheckForNotNaNinPlanes(Ip));
		cv::Mat M0;
		M.copyTo(M0);

		// To-Do Logical indexing

	}

	return true;
}
#endif