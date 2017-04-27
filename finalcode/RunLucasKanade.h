#ifndef RUNLUCASKANADE_IS_INCLUDED
#define RUNLUCASKANADE_IS_INCLUDED

#include "lukaskanade.h"
#include "readImage.h"
#include "readVideo.h"
#include "ComputeDs.h"
#include "generateBitplanes.h"
#include "imageFunctions.h"

bool ValidateInputs(int argc, char** argv, cv::VideoCapture &capVideo)
{
	if (argc < 2)
	{
		std::cout << "Incorrect arguments." << std::endl;
		return false;
	}
	if (!readVideoFile(capVideo, argv[1]))
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
	
	// Create Rect object ???
	cv::Mat_<double> imageFrame;
	int count = 0;

	// Allocate memory for workspace variables :
	double sigma = 11;
	cv::Mat_<double> I;
	std::vector<cv::Mat> Iref_bitPlane, I_bitPlane;

	double epsilon = getEpsilon(), lambda = getLambda(), *weights;
	bool * keep = getKeep("projective");
	cv::Mat_<double> Ds;
	cv::Mat_<double> Mref;

	Eigen::Matrix3d H = Eigen::Matrix3d::Identity();

	while (readImageFrameFromVideo(imageFrame,capVideo))
	{
		I = convertToDouble(convertToGrayScale(imageFrame));
		if (count == 0)
		{
			Iref_bitPlane = generateBitPlanes(I);
			ApplyGaussianFilterOnPlanes(Iref_bitPlane, sigma);
			AddPaddingtoBitPlaneswithNaN(Iref_bitPlane, 2, 2, 2, 2);

			weights = getWeights(Iref_bitPlane[0].rows, Iref_bitPlane[0].cols);
			Ds = ComputeGradientsForWarp(Iref_bitPlane, keep, weights, Mref);
		}
		else
		{
			I_bitPlane = generateBitPlanes(I);
			ApplyGaussianFilterOnPlanes(I_bitPlane, sigma);
			AddPaddingtoBitPlaneswithNaN(I_bitPlane, 2, 2, 2, 2);

			if (!LukasKanade(I_bitPlane, Iref_bitPlane, H, Ds, Mref, weights, keep, epsilon, lambda))
			{
				std::cout << "Lucas Kanade returned false !" << std::endl;
				return false;
			}
		}
		++count;

		// Show imageFrame with rectangle ???
	}
}

#endif
