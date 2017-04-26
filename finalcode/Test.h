#ifndef TEST_IS_INCLUDED
#define TEST_IS_INCLUDED

#include <iostream>

#include "readImage.h"
#include "readVideo.h"
#include "generateBitplanes.h"
#include "imwarp.h"
#include "ds2H.h"
#include "ComputeDs.h"
#include "lukaskanade.h"

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
	auto gray_image = convertToGrayScale(image);
	auto img_src = convertToDouble(gray_image);
	Eigen::Matrix3d W(3, 3); 

	W << 0.7	, -0.7	, 0,
		 0.7	, 0.7	, 0,
		 0		, 0		, 1;

	int out_size[] = { img_src.rows, img_src.cols };
	cv::Mat target = ApplyWarp(img_src, W, img_src.rows, img_src.cols);

	showImage(img_src, "original image");
	showImage(target, "warped image");

	W = Eigen::Matrix3d::Identity(3, 3);
	cv::Mat target2 = ApplyWarp(img_src, W, img_src.rows, img_src.cols);

	return cv::countNonZero(target2 - img_src > 1e-10) == 0;
}

bool testpadImages(cv::Mat &image)
{
	cv::Mat result = AddPaddingToImage(image, 10, 10, 10, 10, 0);
	showImage(result, "Padded Image");

	if (image.cols != result.cols - 20 || image.rows != result.rows - 20)
		return false;
	return true;
}

bool testds2H()
{
	double wts[] = { 1, 1, 1, 1,
					1, 1, 1, 1 };

	double ds[] = { 0, 0, 0, 0,
					 0, 0, 0, 0 };

	auto Hs = ds2Hs(ds, wts);
	auto H = Hs2H(Hs);
	
	return (H == Eigen::Matrix3d::Identity(3, 3));
}

bool testImageSubtract()
{
	cv::Mat_<uint8_t> a(10, 10), b(10, 10);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			a(i, j) = i * 10 + j;
			b(i, j) = j * 10 + i;
		}
	}

	cv::Mat_<double> adoub = convertToDouble(a);
	cv::Mat_<double> bdoub = convertToDouble(b);
	
	cv::Mat_<double> result;
	if (SubtractImages(result, adoub, bdoub))
	{
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				std::cout << result(i, j) << " ";
			}
			std::cout << std::endl;
		}
		return true;
	}
	return false;
}

bool testLucasKanade()
{
	double wts[5], keep[5];

	Eigen::Matrix3d H;
	H << 1, 0, 0,
		0, 1, 0,
		0, 0, 1;

	return LukasKanade(cv::Mat_<double>(), cv::Mat_<double>(), H, cv::Mat_<double>(), cv::Mat_<int>(), 8, wts, keep);
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

void RunTestDs2H()
{
	if (testds2H())
		std::cout << "Ds2H Test Succeeded" << std::endl;
	else
		std::cout << "Ds2H Test Failed" << std::endl;
}

void RunImageTestSubtract()
{
	if (testImageSubtract())
	{
		std::cout << "Image Subtraction Test Succeeded" << std::endl;
	}
	else
	{
		std::cout << "Image Subtraction Failed" << std::endl;
	}
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

	RunTestDs2H();
	RunImageTestSubtract();

	if (testLucasKanade())
	{
		std::cout << "Lucas Kanade Tested" << std::endl;
	}
		
	std::cout << "All Tests Done" << std::endl;
}

#endif