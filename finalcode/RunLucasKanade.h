#ifndef RUNLUCASKANADE_IS_INCLUDED
#define RUNLUCASKANADE_IS_INCLUDED

#include "lukaskanade.h"
#include "readImage.h"
#include "readVideo.h"
#include "ComputeDs.h"
#include "generateBitplanes.h"

bool RunLucasKanade(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Incorrect arguments." << std::endl;
		return false; 
	}
	cv::VideoCapture capVideo;
	if (!readVideoFile(capVideo, argv[1]))
	{
		std::cout << "Could not read video." << std::endl;
		return false;
	}

	// Create Rect object ???

	cv::Mat_<double> imageFrame;
	int count = 0;

	// Allocate memory for workspace variables :
	int sig = 11;
	cv::Mat_<double> I;
	std::vector<cv::Mat_<double>> Iref_bitPlane, I_bitPlane;

	double *keep, epsilon, lambda, *weights;
	int K = 8;
	std::vector<cv::Mat_<double>> Ds;
	cv::Mat_<double> Mref;

	Eigen::Matrix3d H;
	H << 1, 0, 0,
		0, 1, 0,
		0, 0, 1;

	while (readImageFrameFromVideo(imageFrame,capVideo))
	{
		I = convertToDouble(convertToGrayScale(imageFrame));
		if (count == 0)
		{
			Iref_bitPlane = generateBitPlanes(I);

			// Apply Gaussian Filter ???

			for (int i = 0; i < Iref_bitPlane.size(); ++i)
			{
				Iref_bitPlane[i] = AddPaddingToImage(Iref_bitPlane[i], 10, 10, 10, 10, 0);
			}

			epsilon = getEpsilon();
			lambda = getLambda();
			keep = getKeep("projective");
			weights = getWeights(imageFrame.rows,imageFrame.cols);

			// What about Mref ???
			Ds = ComputeGradientsForWarp(Iref_bitPlane, keep, weights);
		}
		else
		{
			I_bitPlane = generateBitPlanes(I);

			// Apply Gaussian Filter ???
			// Same or different Padding ???

			for (int i = 0; i < I_bitPlane.size(); ++i)
			{
				I_bitPlane[i] = AddPaddingToImage(I_bitPlane[i], 10, 10, 10, 10, 0);
			}

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
