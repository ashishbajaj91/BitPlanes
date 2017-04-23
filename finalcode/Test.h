#ifndef TEST_IS_INCLUDED
#define TEST_IS_INCLUDED

#include <iostream>

#include "readImage.h"
#include "readVideo.h"
#include "generateBitplanes.h"
#include "imwarp.h"

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
	auto bitplaneImage = generateBitPlanes(gray_image);

	for (int k = 0; k < 8; k++)
	{
		if (!bitplaneImage[k].data)
			return false;

		std::string winname = "bitplaneImage";
		createNamedWindow( (winname + std::to_string(k)).c_str() );
		cv::Mat temp = 255 * bitplaneImage[k];
		showImage(temp, (winname + std::to_string(k)).c_str());
	}
	return true;
}

bool testimwarp(cv::Mat &image)
{
	auto img_src_f = convertToDouble(image);
	Eigen::Matrix3d W(3, 3); 

	W << 0.7	, -0.7	, 0,
		 0.7	, 0.7	, 0,
		 0		, 0		, 1;

	int out_size[] = { img_src_f.rows, img_src_f.cols };
	cv::Mat target = ApplyWarp(img_src_f, W, out_size);

	showImage(img_src_f, "original image");
	showImage(target, "warped image");
	return true;
}

bool testpadImages(cv::Mat &image)
{
	cv::Mat result = AddPaddingToImage(image, 10, 10, 10, 10, 0);
	showImage(result, "Padded Image");

	if (image.cols != result.cols - 20 || image.rows != result.rows - 20)
		return false;
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

void RunTestImwarp(cv::String filename)
{
	cv::Mat image;
	readImage(image, filename);
	if (testimwarp(image))
		std::cout << "ImWarp Test Succeeded" << std::endl;
	else
		std::cout << "Imwarp Test Failed" << std::endl;
}

void RunTestPadImage(cv::String filename)
{
	cv::Mat image;
	readImage(image, filename);
	if (testpadImages(image))
		std::cout << "Pad Images Test Succeeded" << std::endl;
	else
		std::cout << "Pad Images Failed" << std::endl;
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

		RunTestImwarp(argv[1]);
		cv::waitKey(0);
		destroyWindow("All");

		RunTestPadImage(argv[1]);
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