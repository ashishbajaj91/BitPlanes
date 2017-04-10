#ifndef TEST_IS_INCLUDED
#define TEST_IS_INCLUDED

#include <iostream>

#include "readImage.h"
#include "readVideo.h"
#include "generateBitplanes.h"

bool testReadImage(const cv::String &imagefilename)
{
	cv::Mat image;
	return ReadandShowImage(image, imagefilename);
}

bool testReadVideo(const cv::String &videofilename)
{
	cv::VideoCapture capVideo;
	return ReadAndDisplayVideo(capVideo, videofilename);
}

bool testGenerateBitplanes(cv::Mat &image)
{
	cv::Mat gray_image = convertToGrayScale(image);
	cv::Mat bitplaneImage = generateBitPlanes(gray_image);

	if (!bitplaneImage.data)
		return false;

	createNamedWindow("bitplaneImage");
	showImage(bitplaneImage, "bitplaneImage");
	return true;
}

void RunTestReadImage(cv::String filename)
{
	if (testReadImage(filename))
		std::cout << "Image Read Test Succeeded" << std::endl;
	else
		std::cout << "Image Read Test Failed" << std::endl;
}

void RunTestReadVideo(cv::String videofilename)
{
	if (testReadVideo(videofilename))
		std::cout << "Video Read Test Succeeded" << std::endl;
	else
		std::cout << "Video Read Test Failed" << std::endl;
}

void RunTestGenerateBitplanes(cv::String filename)
{
	cv::Mat image;
	readImage(image, filename);
	if (testGenerateBitplanes(image))
		std::cout << "BitPlanes Test Succeeded" << std::endl;
	else
		std::cout << "BitPlanes Test Failed" << std::endl;
}

void RunTests(int argc, char** argv)
{
	std::cout << "Starting the tests" << std::endl;
	if (argc > 1)
	{
		RunTestReadImage(argv[1]);
		cv::waitKey(0);
		destroyWindow("All");

		RunTestGenerateBitplanes(argv[1]);
		cv::waitKey(0);
		destroyWindow("All");
	}
	if (argc > 2)
	{
		RunTestReadVideo(argv[2]);
		destroyWindow("All");
	}
	std::cout << "All Tests Done" << std::endl;
}

#endif