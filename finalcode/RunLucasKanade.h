#ifndef RUNLUCASKANADE_IS_INCLUDED
#define RUNLUCASKANADE_IS_INCLUDED

#include "lucaskanade.h"
#include "readImage.h"
#include "readVideo.h"
#include "ComputeDs.h"
#include "generateBitplanes.h"
#include "imageFunctions.h"
#include "warpCoordinates.h"

bool ValidateInputs(int argc, char** argv, cv::VideoCapture &capVideo)
{
	if (argc < 3)
	{
		std::cout << "Incorrect arguments." << std::endl;
		return false;
	}
	if (!readVideoFile(capVideo, argv[2]))
	{
		std::cout << "Could not read video." << std::endl;
		return false;
	}
	return true;
}

bool RunLucasKanade(int argc, char** argv)
{
	cv::VideoCapture capVideo;

	if (!ValidateInputs(argc, argv, capVideo))
		return false;
	
	std::cout << "Video Read Successful" << std::endl;
	cv::Mat_<double> imageFrame;
	int count = 0;

	// Allocate memory for workspace variables :
	double sigma = 11;
	std::vector<cv::Mat> Iref_bitPlane;

	double epsilon = getEpsilon(), lambda = getLambda(), weights[8];
	bool * keep = getKeep("projective");
	cv::Mat_<double> Ds;
	cv::Mat_<double> Mref;

	Eigen::Matrix3d H = Eigen::Matrix3d::Identity();

	double inCoords[8] = {	100,100,
							500,100, 
							500,500,
							100,500};

	double warpedCoords[8];

	std::cout << "Initialization Done" << std::endl;
	std::cout << "Starting Tracking" << std::endl;
	while (readImageFrameFromVideo(imageFrame,capVideo))
	{
		cv::Mat_<double> I = convertToDouble(convertToGrayScale(imageFrame));
		if (count == 0)
		{
			Iref_bitPlane = generateBitPlanes(I);
			ApplyGaussianFilterOnPlanes(Iref_bitPlane, sigma);
			AddPaddingtoBitPlaneswithNaN(Iref_bitPlane, 2, 2, 2, 2);

			getWeights(Iref_bitPlane[0].rows, Iref_bitPlane[0].cols, weights);
			Ds = ComputeGradientsForWarp(Iref_bitPlane, keep, weights, Mref);
		}
		else
		{
			auto I_bitPlane = generateBitPlanes(I);
			ApplyGaussianFilterOnPlanes(I_bitPlane, sigma);
			AddPaddingtoBitPlaneswithNaN(I_bitPlane, 2, 2, 2, 2);
			
			if (!LukasKanade(I_bitPlane, Iref_bitPlane, H, Ds, Mref, weights, keep, epsilon, lambda))
			{
				std::cout << "Lucas Kanade returned false !" << std::endl;
				return false;
			}
		}
		std::cout << "Count:" << count << std::endl;
		++count;
		warpCoords(warpedCoords, inCoords, H, I.cols, I.rows, true);
		showImage(drawBoundingBox(warpedCoords, imageFrame), "Current Frame");
		char chCheckForEscKey = cv::waitKey(0);
		if (chCheckForEscKey == 27)
		{
			break;
		}
	}

	destroyWindow("All");
	return true;
}

#endif
