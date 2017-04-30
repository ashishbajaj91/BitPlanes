#ifndef GRADIENT_IS_INCLUDED
#define GRADIENT_IS_INCLUDED

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>

#include "imwarp.h"
#include "padImages.h"
#include "ds2H.h"
#include "matrixFunctions.h"

bool SubtractImages(cv::Mat &result, cv::Mat &image1, cv::Mat &image2)
{
	if (image1.cols != image2.cols || image1.rows != image2.rows || image1.dims != image2.dims)
		return false;
	cv::subtract(image1, image2, result, cv::noArray(), CV_64F);
	return true;
}

bool ComputeAbsoluteDifference(cv::Mat &result, cv::Mat &image1, cv::Mat &image2)
{
	if (image1.cols != image2.cols || image1.rows != image2.rows || image1.dims != image2.dims)
		return false;
	cv::absdiff(image1, image2, result);
	return true;
}

std::vector<cv::Mat> SubtractBitPlanes(std::vector<cv::Mat> &image1 , std::vector<cv::Mat> &image2)
{
	std::vector<cv::Mat> result;
	for (int i = 0; i < image1.size(); i++)
	{
		cv::Mat_<double> diff;
		if (!SubtractImages(diff, image1[i], image2[i]))
			std::cout << "The image data is incorrect for subtraction" << std::endl;
		result.push_back(diff);
	}
	return result;
}

std::vector<cv::Mat> ComputeAbsoluteDifferenceBitPlanes(std::vector<cv::Mat> &image1, std::vector<cv::Mat> &image2)
{
	std::vector<cv::Mat> result;
	for (int i = 0; i < image1.size(); i++)
	{
		cv::Mat_<double> diff;
		if (!ComputeAbsoluteDifference(diff, image1[i], image2[i]))
			std::cout << "The image data is incorrect for subtraction" << std::endl;
		result.push_back(diff);
	}
	return result;
}

cv::Mat ComputeBitPlaneGradient(std::vector<cv::Mat> &image1, std::vector<cv::Mat> &image2)
{
	auto result = SubtractBitPlanes(image1, image2);
	cv::Mat gradient_avg;
	for (int i = 0; i < image1.size(); i++)
	{
		if (i == 0)
			gradient_avg = result[i];
		else
			cv::add(result[i], gradient_avg, gradient_avg);
	}
	gradient_avg /= (1.0*image1.size());
	return gradient_avg;
}

std::vector<cv::Mat> InitializeZeroGradients(cv::Mat image)
{
	int rows = image.rows;
	int cols = image.cols;
	int type = image.type();

	std::vector<cv::Mat> WarpGradients;
	for (int i = 0; i < 8; i++)
	{
		cv::Mat T = cv::Mat(rows, cols, type, cvScalar(0.));
		WarpGradients.push_back(T);
	}

	return WarpGradients;
}

cv::Mat ComputeGradientInX(std::vector<cv::Mat> &Iref)
{
	std::vector <cv::Mat> image1;
	
	for (int i = 0; i < Iref.size(); i++)
	{
		cv::Mat temp = Iref[i].colRange(1, Iref[i].cols);
		cv::Mat temp1 = AddPaddingToImage(temp, 0, 0, 0, 1, 0.0);
		image1.push_back(temp1);
	} 

	return ComputeBitPlaneGradient(image1, Iref);
}

cv::Mat ComputeGradientInY(std::vector<cv::Mat> &Iref)
{
	std::vector <cv::Mat> image1;
	for (int i = 0; i < Iref.size(); i++)
	{
		cv::Mat temp = Iref[i].rowRange(1, Iref[i].rows);
		cv::Mat temp1 = AddPaddingToImage(temp, 0, 1, 0, 0, 0.0);
		image1.push_back(temp1);
	}
	return ComputeBitPlaneGradient(image1, Iref);
}
std::vector<cv::Mat> ApplyWarpToBitPlanes(std::vector<cv::Mat> &Iref, Eigen::Matrix3d &H)
{
	if (H == Eigen::Matrix3d::Identity())
		return Iref;
	return ApplyWarpOnPlanes(Iref, H, Iref[0].rows, Iref[0].cols);
}

cv::Mat ComputeGradientPostWarping(std::vector<cv::Mat> &Iref, Eigen::Matrix3d H)
{
	auto WarpedPlanes = ApplyWarpToBitPlanes(Iref, H);
	return ComputeBitPlaneGradient(WarpedPlanes, Iref);
}

cv::Mat ReshapeDs(std::vector<cv::Mat> &WarpedGradients)
{
	cv::Mat reshaped_gradients;
	for (int i = 0; i < WarpedGradients.size(); i++)
	{
		if (i == 0)
			reshaped_gradients = ReshapeImageToColumn(WarpedGradients[i]);
		else
			cv::hconcat(reshaped_gradients, ReshapeImageToColumn(WarpedGradients[i]), reshaped_gradients);
	}
	return reshaped_gradients;
}

cv::Mat ComputeGradientsForWarp(std::vector<cv::Mat> &Iref, int keep[], double wts[], cv::Mat &Mref)
{	
	std::vector<cv::Mat> WarpGradients;
	int counter = 0;

	double ds[] = { -1.0, -1.0, -1.0, -1.0,
					 -1.0, -1.0, -1.0, -1.0 };

	auto Hs = ds2Hs(ds, wts);

	//Gradient in X direction
	if (keep[0])
	{
		cv::Mat GradientX = ComputeGradientInX(Iref);
		WarpGradients.push_back(GradientX);
		counter++;
	}

	//Gradient in Y direction
	if (keep[1])
	{
		cv::Mat GradientY = ComputeGradientInY(Iref);
		WarpGradients.push_back(GradientY);
		counter++;
	}

	//Compute Gradient for other transformations
	for (; counter < 8; counter++)
	{
		cv::Mat warp_grap = ComputeGradientPostWarping(Iref, Hs[counter]);
		WarpGradients.push_back(warp_grap);
	}

	Mref = CheckForNotNaNinPlanes(WarpGradients);
	return ReshapeDs(WarpGradients);
}
#endif