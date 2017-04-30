#ifndef TEST_IS_INCLUDED
#define TEST_IS_INCLUDED

#include <iostream>
#include "readImage.h"
#include "readVideo.h"
#include "generateBitplanes.h"
#include "imwarp.h"
#include "ds2H.h"
#include "ComputeDs.h"
#include "lucaskanade.h"
#include "imageFunctions.h"
#include "warpCoordinates.h"

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

	auto temp = generateBitPlanes(img_src);
	auto bitplaneImage = ApplyWarpOnPlanes(temp, W, img_src.rows, img_src.cols);
	//auto bitplaneImage = ApplyWarpOnPlanes(generateBitPlanes(img_src), W, img_src.rows, img_src.cols);

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
	cv::Mat NaNMat = CheckForNaN(paddedImage);

	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 12; ++j)
		{
			if (i == 0 || j == 0 || i == 11 || j == 11)
				if (NaNMat.at<uint8_t>(i, j) != 255)
					return false;
			//std::cout << NaNMat.at<uint8_t>(i, j) << " ";
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
				if (NotNaNMat.at<uint8_t>(i, j) != 0)
					return false;
			//std::cout << NotNaNMat.at<uint8_t>(i, j) << " ";
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

bool testInverseWarp(const cv::String &imagefilename)
{
	cv::Mat image;
	readImage(image, imagefilename);

	double inCoords[8] = {	image.cols / 10.0, image.rows/10.0, 
							image.cols / 1.5, image.rows/8.0,
							image.cols / 1.8, image.rows / 1.5,
							image.cols / 10.0, image.rows / 1.8,							
						};

	double warpedCoords[8];

	auto gray_image = convertToGrayScale(image);
	auto img_src = convertToDouble(gray_image);
	Eigen::Matrix3d W(3, 3);

	W <<	0.7,	-0.7,	0,
			0.7,	0.7,	0,
			0,		0,		1;

	int out_size[] = { img_src.rows, img_src.cols };
	cv::Mat target = ApplyWarp(img_src, W, img_src.rows, img_src.cols);

	warpCoords(warpedCoords, inCoords, W, img_src.cols, img_src.rows, true);

	auto temp1 = drawBoundingBox(inCoords, img_src);
	auto temp2 = drawBoundingBox(warpedCoords, target);
	showImage(temp1, "original image");
	showImage(temp2, "warped image");

	//showImage(drawBoundingBox(inCoords, img_src), "original image");
	//showImage(drawBoundingBox(warpedCoords, target), "warped image");
	return true;
}

bool testReshapeImageFunction()
{
	cv::Mat_<double> a(4, 4);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if ( j < 2)
				a(i, j) = 1.0;
			else
				a(i, j) = 0.0;
			//std::cout << a.at<double>(i, j);
		}
		//std::cout << std::endl;
	}
	auto reshaped_image_col = ReshapeImageToColumn(a);
	//for (int i = 0; i < 16; ++i)
	//{
	//	std::cout << reshaped_image_col.at<double>(i,0);
	//}

	if (a.channels() != reshaped_image_col.channels() || reshaped_image_col.rows != a.rows * a.cols && reshaped_image_col.cols != 1)
		return false;

	auto reshaped_image_row = ReshapeImageToRow(a);
	//for (int i = 0; i < 16; ++i)
	//{
	//	std::cout << reshaped_image_row.at<double>(0, i);
	//}

	if (a.channels() != reshaped_image_row.channels() || reshaped_image_row.rows != 1 && reshaped_image_row.cols != a.rows * a.cols)
		return false;
	return true;
}

bool testTranspose(const cv::String &imagefilename)
{
	cv::Mat image;
	readImage(image, imagefilename);
	auto transposed_image = Transpose(image);

	showImage(image, "original image");
	showImage(transposed_image, "tranposed image");

	if (transposed_image.data)
		return true;
	return false;
}

bool testInnerProductImage(const cv::String &imagefilename)
{
	cv::Mat image;
	readImage(image, imagefilename);
	cv::Mat grayimage = convertToGrayScale(image);
	cv::Mat double_image = convertToDouble(grayimage);

	cv::Mat inner_product_image = InnerProduct(double_image,double_image);
	//showImage(double_image, "Grayed Double image");
	//showImage(inner_product_image, "inner product image");
	if (inner_product_image.data && inner_product_image.cols == double_image.cols && inner_product_image.rows == double_image.cols)
		return true;
	return false;
}

bool testComputeGradientForWarp()
{
	cv::Mat_<double> a(5, 5);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (i + j < 5)
				a(i, j) = 1.0;
			else
				a(i, j) = 0.0;
			//std::cout << a(i, j);
		}
		//std::cout << std::endl;
	}
	std::vector<cv::Mat> vec; vec.push_back(a);
	double weights[8]; getWeights(vec[0].rows, vec[0].cols, weights);
	cv::Mat Mref;
	int keep[8]; getKeep("projective", keep);

	cv::Mat_<double> Ds = ComputeGradientsForWarp(vec, keep, weights, Mref);
	
	if(Ds(0,0) == 0 && Ds(0,1) == 0)
		return true;
	return false;
}

//void testgrayscale(const cv::String imagefilename)
//{
//	cv::Mat image;
//	readImage(image, imagefilename);
//	
//	cv::Mat gray_image = convertToDouble(convertToGrayScale(image));
//	gray_image = ApplyGaussianFilter(gray_image, 2.0);
//	std::cout << gray_image.rows << " " << gray_image.cols << std::endl;
//
//	std::vector<cv::Mat> vec; vec.push_back(gray_image);
//	double weights[8]; getWeights(vec[0].rows, vec[0].cols, weights);
//	cv::Mat Mref;
//	int keep[8]; getKeep("projective", keep);
//
//	for (int i = 0; i < vec[0].rows; i++)
//	{
//		for (int j = 0; j < vec[0].cols; j++)
//		{
//			std::cout //<< std::setprecision(2) 
//				<< vec[0].at<double>(i, j) << " ";
//		}
//		std::cout << std::endl;
//	}
//
//
//	std::cout << gray_image.at<double>(5, 0) << " " << gray_image.at<double>(5, 1) 
//		<< gray_image.at<double>(5, 1) - gray_image.at<double>(5, 0)
//		<< std::endl;
//
//	cv::Mat_<double> Ds = ComputeGradientsForWarp(vec, keep, weights, Mref);
//
//	for (int i = 0; i < Ds.rows/4; i++)
//	{
//		for (int j = 0; j < Ds.cols; j++)
//		{
//			std::cout << std::setprecision(2) << Ds.at<double>(i, j) << " ";
//		}
//		std::cout << std::endl;
//	}
//
//	std::cout << "Non NaN: " << cv::countNonZero(Mref) << std::endl;
//}


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

		RunTest(testInverseWarp(argv[1]), "Warped Coordinates");
		cv::waitKey(0);
		destroyWindow("All");

		RunTest(testTranspose(argv[1]), "Image Transpose");
		cv::waitKey(0);
		destroyWindow("All");
		
		RunTest(testInnerProductImage(argv[1]), "Image Inner Product");
		cv::waitKey(0);
		destroyWindow("All");

	}
	/*if (argc > 2)
	{
		RunTest(testReadVideo(argv[2]), "Video Read");
		destroyWindow("All");
	}*/
	RunTest(testds2H(), "Ds2H");
	RunTest(testImageSubtract(), "Image Subtract");
	RunTest(testCheckForNaN(), "Check for NaN");
	RunTest(testCheckForNotNaNinPlanes(), "Not NaN in Plane");
	RunTest(testApplyGaussianFilterOnPlanes(), "Gaussian Filter on Plane");
	RunTest(testReshapeImageFunction(), "Reshape Image");
	RunTest(testComputeGradientForWarp(), "Compute Gradients for Warp");
	std::cout << std::endl << "All Tests Done" << std::endl;
}

#endif