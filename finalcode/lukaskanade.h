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
		auto dI = SubtractBitPlanes(Ip, Iref);
		auto dI0 = ComputeAbsoluteDifferenceBitPlanes(Ip, Iref);

		auto M = cv::Mat(Mref & ~CheckForNotNaNinPlanes(Ip));
		cv::Mat M0;
		M.copyTo(M0);
		
		// To-Do Logical indexing

	}

	return true;
}
#endif