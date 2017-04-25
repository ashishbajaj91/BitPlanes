#ifndef LUKASKANADE_IS_INCLUDED
#define LUKASKANADE_IS_INCLUDED

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "getParameters.h"

void ComputeSVD(double *s, Eigen::Matrix3d H)
{
	Eigen::JacobiSVD<Eigen::Matrix3d> svd(H);
	auto sing = svd.singularValues();
	s[0] = sing[0];
	s[1] = sing[1];
	s[2] = sing[2];
}

bool LukasKanade(cv::Mat_<double> I, cv::Mat_<double> Iref, Eigen::Matrix3d &H, cv::Mat_<double> Ds, cv::Mat_<int> Mref, int K, double *wts, double *keep)
{
	const double epsilon = getEpsilon();
	double error = std::numeric_limits<double>::quiet_NaN();;
	double ds[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int h = I.rows - 2, w = I.cols - 2;
	cv::Mat_<double> lambda = getLambda()*cv::Mat_<double>::eye(K,K);

	for (int i = 0; i < 100; ++i)
	{
		double s[3];	// Assuming H is always gonna be 3x3 Matrix
		ComputeSVD(s, H);

		if (s[2] <= s[0] * 1e-4)
		{
			H << 1, 0, 0,
				0, 1, 0,
				0, 0, 1;
			return true;
		}

		cv::Mat_<double> Ip = ApplyWarp(I, H, I.rows, I.cols);
		cv::Mat_<double> dI = Ip - Iref;
		cv::Mat_<double> dI0 = cv::abs(dI);

		// To-Do Logical indexing

	}

	return true;
}
#endif