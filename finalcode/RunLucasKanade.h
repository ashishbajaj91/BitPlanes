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
	//Make sure we are using optimized version
	cv::setUseOptimized(true);

	cv::VideoCapture capVideo;
	if (!ValidateInputs(argc, argv, capVideo))
		return false;
	
	std::cout << "Video Read Successful" << std::endl;
	cv::Mat_<double> imageFrame;
	int count = 0;

	double sigma = 11.0;
	std::vector<cv::Mat> Iref;

	double epsilon = getEpsilon(), lambdathreshold = getLambda(), weights[8];
	int keep[8];  getKeep("projective", keep);
	cv::Mat_<double> Ds;
	cv::Mat Mref;

	int useGrayScale = 1;
	if (argc > 3)
		useGrayScale = atoi(argv[3]);

	int saveImage = 0;
	if (argc > 4)
		saveImage = atoi(argv[4]);


	if (useGrayScale)
		std::cout << "Running with Gray Scale";
	else
		std::cout << "Running with Bit Planes";
	std::cout << std::endl;

	Eigen::Matrix3d H = Eigen::Matrix3d::Identity();
	cv::Mat_<double> lambda;


	double inCoords[8] = {	218,94,
							910,89, 
							908,655,
							212,651};

	//cv::Rect AreaOfInterest(212, 89, 910 - 212, 655 - 89);
	cv::Rect AreaOfInterest(212, 89, 698, 566);

	double warpedCoords[8];

	std::cout << "Initialization Done" << std::endl;
	std::cout << "Starting Tracking" << std::endl;
	createNamedWindow("Current Frame");
	while (readImageFrameFromVideo(imageFrame,capVideo))
	{
		auto gray_image = convertToGrayScale(imageFrame);
		cv::Mat_<double> I = convertToDouble(gray_image);
		//cv::Mat_<double> I = convertToDouble(convertToGrayScale(imageFrame));
		if (count == 0)
		{
			std::vector<cv::Mat> Iref_bitPlane;
			if(!useGrayScale)
				Iref_bitPlane = generateBitPlanes(I);
			else
				Iref_bitPlane.push_back(I);
			ApplyGaussianFilterOnPlanes(Iref_bitPlane, sigma);
			//AddPaddingtoBitPlaneswithNaN(Iref_bitPlane, 2, 2, 2, 2);
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
			//AddPaddingtoBitPlaneswithNaN(I_bitPlane, 2, 2, 2, 2);
			
			if (!LukasKanade(I_bitPlane, Iref, H, Ds, Mref, weights, keep, epsilon, lambda, AreaOfInterest))
			{
				std::cout << "Lucas Kanade returned false !" << std::endl;
				return false;
			}
		}
		//if(count % 10 == 0)
			std::cout << "Count:" << count << std::endl;
		++count;
		
		warpCoords(warpedCoords, inCoords, H, imageFrame.cols, imageFrame.rows, true);

		cv::Mat image = drawBoundingBox(warpedCoords, imageFrame);

		if(saveImage)
			cv::imwrite("img_" + std::to_string(count) + ".png", image);

		cv::Size newSize(imageFrame.cols / 2, imageFrame.rows / 2);
		cv::resize(image, image, newSize);
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