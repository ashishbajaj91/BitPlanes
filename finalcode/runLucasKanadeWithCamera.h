#ifndef RUNLUCASKANADEWITHCAMERA_IS_INCLUDED
#define RUNLUCASKANADEWITHCAMERA_IS_INCLUDED

#include "opencv2/opencv.hpp"
#include "runLucasKanade.h"

bool RunLucasKanadeWithCamera()
{
	//Make sure we are using optimized version
	cv::setUseOptimized(true);

	cv::VideoCapture cap(0);
	if (!cap.isOpened()) 
	{
		std::cout << "Unable to open the camera" << std::endl;
		return false;
	}
	int count = 0;
	double sigma = 11.0;

	double epsilon = getEpsilon(), lambdathreshold = getLambda(), weights[8];
	int keep[8];  getKeep("projective", keep);
	
	cv::Mat_<double> Ds; cv::Mat Mref;
	int useGrayScale = 1;

	Eigen::Matrix3d H = Eigen::Matrix3d::Identity();
	cv::Mat_<double> lambda;

	cv::Mat imageFrame;
	cap >> imageFrame;

	std::vector<cv::Mat> Iref;

	double inCoords[8] = {	0.3*imageFrame.cols, 0.3*imageFrame.rows,
							0.7*imageFrame.cols, 0.3*imageFrame.rows,
							0.7*imageFrame.cols, 0.7*imageFrame.rows,
							0.3*imageFrame.cols, 0.7*imageFrame.rows
						};

	int x = (0.3*imageFrame.cols);
	int y = (0.3*imageFrame.rows);
	int w = (0.4*imageFrame.cols);
	int h = (0.4*imageFrame.rows);

	cv::Rect AreaOfInterest(x,y,w,h);

	double warpedCoords[8];
	std::cout << "Initialization Done" << std::endl;
	std::cout << "Starting Tracking" << std::endl;
	createNamedWindow("Current Frame");
	while(1)
	{
		cap >> imageFrame;
		auto gray_image = convertToGrayScale(imageFrame);
		cv::Mat_<double> I = convertToDouble(gray_image);
		if (count == 0)
		{
			std::vector<cv::Mat> Iref_bitPlane;
			if (!useGrayScale)
				Iref_bitPlane = generateBitPlanes(I);
			else
				Iref_bitPlane.push_back(I);
			ApplyGaussianFilterOnPlanes(Iref_bitPlane, sigma);
			Iref = ExtractAreaOfInterestFromPlanes(Iref_bitPlane, AreaOfInterest);
			getWeights(Iref[0].rows, Iref[0].cols, weights);
			Ds = ComputeGradientsForWarp(Iref, keep, weights, Mref);
			lambda = Iref[0].rows*Iref[0].cols*lambdathreshold*cv::Mat_<double>::eye(8, 8);
		}
		else
		{
			std::vector<cv::Mat> I_bitPlane;
			if (!useGrayScale)
				I_bitPlane = generateBitPlanes(I);
			else
				I_bitPlane.push_back(I);
			ApplyGaussianFilterOnPlanes(I_bitPlane, sigma);

			if (!LukasKanade(I_bitPlane, Iref, H, Ds, Mref, weights, keep, epsilon, lambda, AreaOfInterest))
			{
				std::cout << "Lucas Kanade returned false !" << std::endl;
				return false;
			}
		}
		std::cout << "Count:" << count << std::endl;
		++count;

		warpCoords(warpedCoords, inCoords, H, imageFrame.cols, imageFrame.rows, true);
		cv::Mat image = drawBoundingBox(warpedCoords, imageFrame);
		showImage(image, "Current Frame");
		char chCheckForEscKey = cv::waitKey(1);
		if (chCheckForEscKey == 27)
		{
			break;
		}
	}
	destroyWindow("All");
	return true;
}
#endif