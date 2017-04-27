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
#include "imageFunctions.h"

void RunTest(bool result, std::string message)
{
	if (result)
		std::cout << message + " Test Succeeded" << std::endl;
	else
		std::cout << message + " Test Failed" << std::endl;
}

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

bool testGenerateBitplanes(const cv::String &imagefilename)
{
	cv::Mat image;
	readImage(image, imagefilename);

	cv::Mat gray_image = convertToGrayScale(image);
	cv::Mat double_image = convertToDouble(gray_image);
	auto bitplaneImage = generateBitPlanes(double_image);

	for (int k = 0; k < 8; k++)
	{
		if (!bitplaneImage[k].data)
			return false;

		std::string winname = "bitplaneImage";
		createNamedWindow( (winname + std::to_string(k)).c_str() );
		auto temp = bitplaneImage[k];
		showImage(temp, (winname + std::to_string(k)).c_str());
	}
	return true;
}

bool testimwarp(const cv::String &imagefilename)
{
	cv::Mat image;
	readImage(image, imagefilename);

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

	auto bitplaneImage = ApplyWarpOnPlanes(generateBitPlanes(img_src), W, img_src.rows, img_src.cols);

	for (int k = 0; k < 8; k++)
	{
		if (!bitplaneImage[k].data)
			return false;

		std::string winname = "bitplaneImage";
		createNamedWindow((winname + std::to_string(k)).c_str());
		auto temp = bitplaneImage[k];
		showImage(temp, (winname + std::to_string(k)).c_str());
	}

	W = Eigen::Matrix3d::Identity(3, 3);
	cv::Mat target2 = ApplyWarp(img_src, W, img_src.rows, img_src.cols);

	return cv::countNonZero(target2 - img_src > 1e-10) == 0;
}

bool testpadImages(const cv::String &imagefilename)
{
	cv::Mat image;
	readImage(image, imagefilename);

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
	
	cv::Mat result;
	if (SubtractImages(result, a, b))
	{
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				if (i > j && result.at<double>(i, j) < 0.0)
					return false;
				else if (i < j && result.at<double>(i, j) > 0.0)
					return false;
				//std::cout << result.at<double>(i, j) << " ";
			}
			//std::cout << std::endl;
		}
		return true;
	}
	return false;
}

bool testReshapeDs(const cv::String &imagefilename)
{
	cv::Mat image;
	readImage(image, imagefilename);

	cv::Mat gray_image = convertToGrayScale(image);
	cv::Mat double_image = convertToDouble(gray_image);
	auto bitplaneImage = generateBitPlanes(double_image);
	auto rehaped_image = ReshapeDs(bitplaneImage);

	if (rehaped_image.cols == bitplaneImage.size() && rehaped_image.rows == bitplaneImage[0].rows * bitplaneImage[0].cols
		&& rehaped_image.channels() == 1)
		return true;
	return false;
}

bool testCheckForNaN()
{
	cv::Mat_<double> a(10, 10);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			a(i, j) = i * 10 + j;
		}
	}

	auto paddedImage = AddPaddingToImage(a, 1, 1, 1, 1, fNaN);
	auto NaNMat = CheckForNaN(paddedImage);
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			if (i == 0 || j == 0 || i == 11 || j == 11)
				if (NaNMat.at<double>(i, j) != 1.0)
					return false;
			//std::cout << NaNMat.at<double>(i, j) << " ";
		}
		//std::cout << std::endl;
	}
	return true;
}

bool testCheckForNotNaNinPlanes()
{
	cv::Mat_<double> a(10, 10);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			a(i, j) = i * 10 + j;
		}
	}

	auto bitPlanes = generateBitPlanes(a);
	bitPlanes[0] = AddPaddingToImage(bitPlanes[0], 1, 1, 1, 1, fNaN);
	for (int i = 1; i < bitPlanes.size(); i++)
	{
		bitPlanes[i] = AddPaddingToImage(bitPlanes[i], 1, 1, 1, 1, 1);
	}

	auto NotNaNMat = CheckForNotNaNinPlanes(bitPlanes);
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			if (i == 0 || j == 0 || i == 11 || j == 11)
				if (NotNaNMat.at<double>(i, j) != 0.0)
					return false;
			//std::cout << NotNaNMat.at<double>(i, j) << " ";
		}
		//std::cout << std::endl;
	}
	return true;
}

bool testApplyGaussianFilterOnPlanes()
{
	cv::Mat_<double> a(10, 10);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if(i + j < 10)
				a(i, j) = 1.0;
			else
				a(i, j) = 0.0;
		}
	}
	auto b = ApplyGaussianFilter(a, 1.0);
	if (b.data)
		return true;
	/*for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			std::cout << b.at<double>(i, j) << " ";
		}
		std::cout << std::endl;
	}*/
	return true;
}

void RunTests(int argc, char** argv)
{
	std::cout << "Starting the tests" << std::endl << std::endl;
	if (argc > 1)
	{
		RunTest(testReadImage(argv[1]), "Image Read");
		cv::waitKey(0);
		destroyWindow("All");

		RunTest(testGenerateBitplanes(argv[1]), "BitPlanes Generation");
		cv::waitKey(0);
		destroyWindow("All");

		RunTest(testimwarp(argv[1]), "ImWarp");
		cv::waitKey(0);
		destroyWindow("All");

		RunTest(testpadImages(argv[1]), "Pad Images");
		cv::waitKey(0);
		destroyWindow("All");

		RunTest(testReshapeDs(argv[1]), "Reshape Ds");
		cv::waitKey(0);
		destroyWindow("All");

	}
	if (argc > 2)
	{
		RunTest(testReadVideo(argv[2]), "Video Read");
		destroyWindow("All");
	}
	RunTest(testds2H(), "Ds2H");
	RunTest(testImageSubtract(), "Image Subtract");
	RunTest(testCheckForNaN(), "Check for NaN");
	RunTest(testCheckForNotNaNinPlanes(), "Not NaN in Plane");
	RunTest(testApplyGaussianFilterOnPlanes(), "Gaussian Filter on Plane");
	std::cout << std::endl << "All Tests Done" << std::endl;
}

#endif