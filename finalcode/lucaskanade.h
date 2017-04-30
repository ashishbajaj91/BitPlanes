#ifndef LUCASKANADE_IS_INCLUDED
#define LUCASKANADE_IS_INCLUDED

#include <iostream>
#include <thread>
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

void ComputeNotNaNdI(cv::Mat *NotNaNdI, cv::Mat *dI, cv::Mat *M)
{
	(*dI).setTo(0.0, ~(*M));
	*NotNaNdI = ReshapeImageToColumn(*dI);
	return;
}

void ComputeNotNaNDs(cv::Mat *NotNaNDs, cv::Mat *Ds, cv::Mat *M)
{
	(*Ds).copyTo(*NotNaNDs);
	(*NotNaNDs).setTo(0.0, ~AddPaddingToImage(ReshapeImageToColumn(*M), 0, 0, 0, (*Ds).cols - 1));
	return;
}


void Computeds(cv::Mat Ds, cv::Mat &M, cv::Mat &dI, cv::Mat &lambda, double ds[], double factor = 1.0)
{
	//cv::Mat_<double> NotNaNdI;
	//dI.copyTo(NotNaNdI);
	//NotNaNdI.setTo(0.0, ~M);

	std::thread multi_threads[2];

	cv::Mat_<double> NotNaNdI;
	multi_threads[0] = std::thread(ComputeNotNaNdI, &NotNaNdI, &dI, &M);
	//dI.setTo(0.0, ~M);
	//cv::Mat_<double> NotNaNdI = ReshapeImageToColumn(dI);

	cv::Mat_<double> NotNaNDs;
	multi_threads[1] = std::thread(ComputeNotNaNDs, &NotNaNDs, &Ds, &M);

	multi_threads[0].join();
	multi_threads[1].join();
	//Ds.copyTo(NotNaNDs);
	//NotNaNDs.setTo(0.0, ~AddPaddingToImage(ReshapeImageToColumn(M), 0, 0, 0, Ds.cols - 1));
	//Ds.setTo(0.0, ~AddPaddingToImage(ReshapeImageToColumn(M), 0, 0, 0, Ds.cols - 1));

	cv::Mat_<double> dsMat =  ((InnerProduct(NotNaNDs, NotNaNDs) + lambda).inv() * (InnerProduct(NotNaNDs, NotNaNdI))) / factor;
	//cv::Mat_<double> dsMat = ((InnerProduct(Ds, Ds) + lambda).inv() * (InnerProduct(Ds, NotNaNdI))) / factor;
	UpdatedsFromMat(dsMat, ds);
}

cv::Mat ComputeError(std::vector<cv::Mat> &Ip, std::vector<cv::Mat> &Iref)
{
	cv::Mat_<double> dI;
	auto errorPlanes = ComputeAbsoluteDifferenceBitPlanes(Ip, Iref);

	//Adding this since this is faster
	if (8 == errorPlanes.size())
	{
		dI = errorPlanes[0] + errorPlanes[1] + errorPlanes[2] + errorPlanes[3] + errorPlanes[4] + errorPlanes[5] + errorPlanes[6] + errorPlanes[7];
		return dI;
	}

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

	//Adding this since this is faster
	if (8 == errorPlanes.size())
	{
		dI = errorPlanes[0] + errorPlanes[1] + errorPlanes[2] + errorPlanes[3] + errorPlanes[4] + errorPlanes[5] + errorPlanes[6] + errorPlanes[7];
		return dI;
	}

	for (int i = 0; i < errorPlanes.size(); i++)
	{
		if (i == 0)
			dI = errorPlanes[i];
		else
			dI = dI + errorPlanes[i];
	}
	return dI;
}

void UpdateDsWithKeep(int * keep, double ds[])
{
	for (int i = 0; i < 8; i++)
	{
		if (!keep[i])
		{
			ds[i] = 0;
		}
	}
}

double ComputeMeanError(cv::Mat &dI0, cv::Mat &M, double factor = 1.0)
{
	//cv::Mat result;
	//dI0.copyTo(result);
	dI0.setTo(0.0, ~M);
	return cv::mean(dI0)[0] / factor;
}

void ComputeDifferenceAndAbsoluteDifference(cv::Mat &dI, cv::Mat &dI0, std::vector<cv::Mat> &Ip, std::vector<cv::Mat> &Iref)
{
	auto errorPlanes = SubtractBitPlanes(Ip, Iref);
	//Adding this since this is faster
	if (8 == errorPlanes.size())
	{
		dI = errorPlanes[0] + errorPlanes[1] + errorPlanes[2] + errorPlanes[3] + errorPlanes[4] + errorPlanes[5] + errorPlanes[6] + errorPlanes[7];
		dI0 =	abs(errorPlanes[0]) +
				abs(errorPlanes[1]) +
				abs(errorPlanes[2]) +
				abs(errorPlanes[3]) +
				abs(errorPlanes[4]) +
				abs(errorPlanes[5]) +
				abs(errorPlanes[6]) +
				abs(errorPlanes[7]);
	}

	for (int i = 0; i < errorPlanes.size(); i++)
	{
		if (i == 0)
		{
			dI = errorPlanes[i];
			dI0 = abs(errorPlanes[i]);
		}
		else
		{
			dI = dI + errorPlanes[i];
			dI0 = dI0 + abs(errorPlanes[i]);
		}
	}
}

void ComputedI(cv::Mat *dI, std::vector<cv::Mat> *Ip, std::vector<cv::Mat> *Iref)
{
	*dI = ComputeSumedSubtraction(*Ip, *Iref);
	return;
}

void ComputedI0(cv::Mat *dI0, std::vector<cv::Mat> *Ip, std::vector<cv::Mat> *Iref)
{
	*dI0 = ComputeError(*Ip, *Iref);
	return;
}

void ComputeM(cv::Mat *M, cv::Mat *Mref, std::vector<cv::Mat> *Ip)
{
	*M = (*Mref & CheckForNotNaNinPlanes(*Ip));
	return;
}

bool LukasKanade(std::vector<cv::Mat> &I, std::vector<cv::Mat> &Iref, Eigen::Matrix3d &H,
				 cv::Mat &Ds, cv::Mat Mref, 
				 double *wts, int *keep, double epsilon, cv::Mat lambda)
{
	double error = std::numeric_limits<double>::infinity();
	double ds[8] = {0, 0, 0, 0, 
					0, 0, 0, 0};

	std::thread multi_threads[3];
	//Initializeds(ds);
	for (int i = 0; i < 100; ++i)
	{
		clock_t begin = clock();

		auto Ip = ApplyWarpToBitPlanes(I, H);
		cv::Mat dI; // = ComputeSumedSubtraction(Ip, Iref);
		cv::Mat dI0; // = ComputeError(Ip, Iref);
		//ComputedI(dI, Ip, Iref);
		//ComputedI0(dI0, Ip, Iref);
		cv::Mat M;	// = (Mref & CheckForNotNaNinPlanes(Ip));

		multi_threads[0] = std::thread(ComputedI, &dI, &Ip, &Iref);
		multi_threads[1] = std::thread(ComputedI0, &dI0, &Ip, &Iref);

		//This is not fast enough.. Not to be used
		//ComputeDifferenceAndAbsoluteDifference(dI, dI0, Ip, Iref);

		multi_threads[2] = std::thread(ComputeM, &M, &Mref, &Ip);

		multi_threads[0].join();
		multi_threads[2].join();

		Computeds(Ds, M, dI, lambda, ds, 1.0*I.size());
		UpdateDsWithKeep(keep, ds);

		H = H * Hs2H(ds2Hs(ds, wts));
		H /= H(2, 2);

		auto error0 = error;

		multi_threads[1].join();
		error = ComputeMeanError(dI0, M, 1.0*I.size());
		std::cout << "Current Error:" << error << std::endl;

		clock_t end = clock();
		double sec_diff = double(end - begin) / CLOCKS_PER_SEC;
		std::cout << "diff: " << sec_diff << std::endl;

		if ((error0 - error) < epsilon)
			break;
	}
	return true;
}
#endif