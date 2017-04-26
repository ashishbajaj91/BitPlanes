#ifndef GRADIENT_IS_INCLUDED
#define GRADIENT_IS_INCLUDED

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "imwarp.h"
#include "padImages.h"
#include "ds2H.h"

bool SubtractImages(cv::Mat_<double> &result, cv::Mat_<double> &image1, cv::Mat_<double> &image2)
{
	if (image1.cols != image2.cols || image1.rows != image2.rows || image1.dims != image2.dims)
		return false;
	cv::subtract(image1, image2, result);

	return true;
}

bool SubtractImages(cv::Mat &result, cv::Mat &image1, cv::Mat &image2)
{
	if (image1.cols != image2.cols || image1.rows != image2.rows || image1.dims != image2.dims)
		return false;
	cvAbsDiff(&image1, &image2, &result);
	return true;
}

std::vector<cv::Mat> SubtractBitPlanes(std::vector<cv::Mat> &image1 , std::vector<cv::Mat> &image2)
{
	std::vector<cv::Mat> result;
	for (int i = 0; i < image1.size(); i++)
	{
		cv::Mat diff;
		if (!SubtractImages(diff, image1[i], image2[i]))
			std::cout << "The image data is incorrect for subtraction" << std::endl;
		result.push_back(diff);
	}
}

cv::Mat ComputeBitPlaneGradient(std::vector<cv::Mat> &image1, std::vector<cv::Mat> &image2)
{
	auto result = SubtractBitPlanes(image1, image2);
	cv::Mat gradient_avg;
	for (int i = 0; i < 8; i++)
	{
		if (i == 0)
			gradient_avg = result[i];
		else
			gradient_avg += result[i];
	}
	gradient_avg /= 8;
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
	std::vector <cv::Mat> image1, image2;
	
	for (int i = 0; i < 8; i++)
	{
		cv::Mat temp1 = Iref[i].colRange(1, Iref[i].cols);
		cv::Mat temp2 = Iref[i].colRange(0, Iref[i].cols - 1);
		image1.push_back(temp1);
		image2.push_back(temp2);
	} 

	return ComputeBitPlaneGradient(image1, image2);
}

cv::Mat ComputeGradientInY(std::vector<cv::Mat> &Iref)
{
	std::vector <cv::Mat> image1, image2;

	for (int i = 0; i < 8; i++)
	{
		cv::Mat temp1 = Iref[i].rowRange(1, Iref[i].rows);
		cv::Mat temp2 = Iref[i].colRange(0, Iref[i].rows - 1);
		image1.push_back(temp1);
		image2.push_back(temp2);
	}
	return ComputeBitPlaneGradient(image1, image2);
}

cv::Mat ComputeGradientPostWarping(std::vector<cv::Mat> &Iref, Eigen::Matrix3d H)
{
	std::vector<cv::Mat> WarpedPlanes;
	for (int i = 0; i < 8; i++)
	{
		cv::Mat temp = ApplyWarp(Iref[i], H, Iref[i].rows, Iref[i].cols);
		WarpedPlanes.push_back(temp);
	}
	return ComputeBitPlaneGradient(WarpedPlanes, Iref);
}

std::vector<cv::Mat> ComputeGradientsForWarp(std::vector<cv::Mat> &Iref, double keep[], double wts[])
{	
	std::vector<cv::Mat> WarpGradients; //= InitializeZeroGradients(Iref[0]);
	int counter = 0;

	double ds[] = { 1, 1, 1, 1,
					 1, 1, 1, 1 };

	auto Hs = ds2Hs(ds, wts);

	//Gradient in X direction
	if (keep[0] > 1e-6)
	{
		cv::Mat GradientX = ComputeGradientInX(Iref);
		WarpGradients.push_back(AddPaddingToImage(GradientX, 0, 0, 0, 1, 0));
		counter++;
	}
	//Gradient in Y direction
	if (keep[1] > 1e-6)
	{
		cv::Mat GradientY = ComputeGradientInY(Iref);
		WarpGradients.push_back(AddPaddingToImage(GradientY, 0, 1, 0, 0, 0));
		counter++;
	}
	//Compute Gradient for other transformations
	for (; counter < 8; counter++)
	{
		cv::Mat warp_grap = ComputeGradientPostWarping(Iref, Hs[counter]);
		WarpGradients.push_back(warp_grap);
	}
}

#endif