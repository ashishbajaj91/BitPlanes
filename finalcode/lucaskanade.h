#ifndef LUCASKANADE_IS_INCLUDED
#define LUCASKANADE_IS_INCLUDED

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "getParameters.h"
#include "matrixFunctions.h"

void Initializeds(double ds[8])
{
	for (int i = 0; i < 8; i++)
		ds = 0;
}

cv::Mat ReshapeImageToColumn(cv::Mat &image)
{
	cv::Mat result = image.reshape(image.channels(), image.rows * image.cols);
	return result;
}

cv::Mat ReshapeImageToRow(cv::Mat &image)
{
	cv::Mat result = image.reshape(image.channels(), 1);
	return result;
}

cv::Mat Transpose(cv::Mat &image)
{
	cv::Mat result;
	cv::transpose(image, result);
	return result;
}

cv::Mat InnerProduct(cv::Mat mat1, cv::Mat mat2)
{
	return Transpose(mat1) * (mat2);
}

void UpdatedsFromMat(cv::Mat &dsMat, double ds[])
{
	for (int i = 0; i < 8; i++)
	{
		if (dsMat.at<double>(i, 0) != dsMat.at<double>(i, 0))
		{
			Initializeds(ds);
			return;
		}
		ds[i] = dsMat.at<double>(i, 0);
	}
}

void Computeds(cv::Mat Ds, cv::Mat &M, cv::Mat &dI, cv::Mat &lambda, double ds[])
{
	cv::Mat_<double> NotNaNdI;
	dI.copyTo(NotNaNdI);

	std::cout << "0" << std::endl;

	NotNaNdI.setTo(0.0, ~M);
	NotNaNdI = ReshapeImageToColumn(NotNaNdI);

	std::cout << "1" << std::endl;

	cv::Mat_<double> NotNaNDs;
	Ds.copyTo(NotNaNDs);
	NotNaNDs.setTo(0.0, ~AddPaddingToImage(ReshapeImageToColumn(M), 0, 0, 0, Ds.cols - 1));

	std::cout << "2" << std::endl;

	cv::Mat_<double> dsMat = ((InnerProduct(NotNaNDs, NotNaNDs) + lambda).inv()) * (InnerProduct(NotNaNDs, NotNaNdI))/64;
	UpdatedsFromMat(dsMat, ds);
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

void UpdateDsWithKeep(bool * keep, double ds[])
{
	for (int i = 0; i < 8; i++)
	{
		if (keep[i] == false)
			ds[i] = 0;
	}
}

double ComputeSummedError(cv::Mat &dI0, cv::Mat &M)
{
	cv::Mat result;
	cv::multiply(dI0, M, result);
	return cv::sum(result)[0];
}

double ComputeMeanError(cv::Mat &dI0, cv::Mat &M)
{
	cv::Mat result;
	dI0.copyTo(result);
	result.setTo(0.0, ~M);
	return cv::mean(result)[0];
}

bool LukasKanade(std::vector<cv::Mat> &I, std::vector<cv::Mat> &Iref, Eigen::Matrix3d &H,
				 cv::Mat &Ds, cv::Mat Mref, 
				 double *wts, bool *keep, double epsilon, double lambdathreshold)
{
	double error = std::numeric_limits<double>::infinity();
	double ds[8];
	Initializeds(ds);

	double h = I[0].rows - 2;
	double w = I[0].cols - 2;

	cv::Mat_<double> lambda = h*w*lambdathreshold*cv::Mat_<double>::eye(8,8);

	for (int i = 0; i < 100; ++i)
	{

		auto Ip = ApplyWarpToBitPlanes(I, H);
		auto dI = ComputeSumedSubtraction(Ip, Iref);

		auto dI0 = ComputeError(Ip, Iref);
		dI0 /= 8;

		cv::Mat M = (Mref & CheckForNotNaNinPlanes(Ip));

		Computeds(Ds, M, dI, lambda, ds);

		UpdateDsWithKeep(keep, ds);

		H = H * Hs2H(ds2Hs(ds, wts));
		auto error0 = error;
		error = ComputeMeanError(dI0, M);
		std::cout << "Current Error:" << error << std::endl;
		if ((error0 - error) < epsilon)
			break;
	}

	return true;
}
#endif