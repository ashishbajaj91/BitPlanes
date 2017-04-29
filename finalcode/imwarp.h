#ifndef IMWARP_IS_INCLUDED
#define IMWARP_IS_INCLUDED

#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <cmath>

typedef double imwarpdatatype;

imwarpdatatype fNaN = std::numeric_limits<imwarpdatatype>::quiet_NaN();

template <class T>
T Interpolate(cv::Mat &image, imwarpdatatype  y, imwarpdatatype  x)
{
	imwarpdatatype xd, yd;

	int xi = int(floor(x));
	int yi = int(floor(y));

	imwarpdatatype k1 = modf(x, &xd); // k1 is the fractional, xd is the integer of x
	imwarpdatatype k2 = modf(y, &yd);

	//std::cout << "Here9.6" << std::endl;

	// Check whether the pixels are within the image
	if (x >= 0.0 && (x <= (1.0*image.cols-1)) && y >= 0.0 && (y <= (1.0*image.rows-1)))
	{
		int f1 = xi <= image.cols - 2; // Check that pixels to the right
		int f2 = yi <= image.rows - 2; // and to down direction exist.
		T px1 = image.at<T>(yi, xi);
		T px2 = image.at<T>(yi, xi + 1);
		T px3 = image.at<T>(yi + 1, xi);
		T px4 = image.at<T>(yi + 1, xi + 1);
		T interpolated_value = (1.0 - k1) * (1.0 - k2) * px1 +
			(f1 ? (k1 * (1.0 - k2) * px2) : 0) +
			(f2 ? ((1.0 - k1) * k2 * px3) : 0) +
			((f1 && f2) ? (k1 * k2 * px4) : 0);
		return interpolated_value;
	}
	else
	{
		return fNaN;
	}
}

cv::Mat InitializeTargetImage(int nrows, int ncols, int imagetype, cv::Scalar scalar)
{
	return cv::Mat(nrows, ncols, imagetype, scalar);
}

cv::Mat ApplyWarp(cv::Mat &imgSrc, Eigen::Matrix3d warpMat, int targetRows, int targetCols)
{
	int imageWidth = imgSrc.cols;
	int imageHeight = imgSrc.rows;

	cv::Mat target = InitializeTargetImage(targetRows, targetCols, imgSrc.type(), cv::Scalar(fNaN));
	warpMat = warpMat.inverse().eval();
	warpMat /= warpMat(2, 2);

	Eigen::Vector3d X;      // Point in coordinate frame of source.
	Eigen::Vector3d U;      // Point in coordinate frame of target.

	for (int v = 0; v < targetRows; v++)
	{
		for (int u = 0; u < targetCols; u++)
		{
			U = Eigen::Vector3d(v - ((imageHeight + 1) / 2.0 - 1), u - ((imageWidth + 1) / 2.0 - 1), 1.0);
			X = warpMat * U;
			imwarpdatatype  y = X(0) / X(2);
			imwarpdatatype  x = X(1) / X(2);
			imwarpdatatype  I2 = Interpolate<imwarpdatatype >(imgSrc, imwarpdatatype(y + ((imageHeight + 1) / 2.0 - 1)),
				imwarpdatatype(x + ((imageWidth + 1) / 2.0) - 1));
			target.at<imwarpdatatype>(v, u) = I2;
		}
	}
	return target;
}

std::vector<cv::Mat> ApplyWarpOnPlanes(std::vector<cv::Mat> &imgSrc, Eigen::Matrix3d warpMat, int targetRows, int targetCols)
{
	int imageWidth = imgSrc[0].cols;
	int imageHeight = imgSrc[0].rows;

	std::vector<cv::Mat> target;

	for (int k = 0; k < imgSrc.size(); k++)
	{
		cv::Mat tarImage = InitializeTargetImage(targetRows, targetCols, imgSrc[0].type(), cv::Scalar(fNaN));
		target.push_back(tarImage);
	}
	warpMat = warpMat.inverse().eval();
	warpMat /= warpMat(2, 2);

	//std::cout << "Here9.2" << std::endl;

	Eigen::Vector3d X;      // Point in coordinate frame of source.
	Eigen::Vector3d U;      // Point in coordinate frame of target.

	//std::cout << "Here9.3" << std::endl;

	for (int v = 0; v < targetRows; v++)
	{
		for (int u = 0; u < targetCols; u++)
		{
			U = Eigen::Vector3d(v - ((imageHeight + 1) / 2.0 - 1), u - ((imageWidth + 1) / 2.0 - 1), 1.0);
			X = warpMat * U;
			imwarpdatatype  y = X(0) / X(2);
			imwarpdatatype  x = X(1) / X(2);
			//if(v==0 && u ==0) 	std::cout << "Here9.4" << std::endl;

			for (int k = 0; k < imgSrc.size(); k++)
			{
				//if (v == 0 && u == 0 ) 	std::cout << "Here9.5" << std::endl;

				imwarpdatatype  I2 = Interpolate<imwarpdatatype >(imgSrc[k], imwarpdatatype(y + ((imageHeight + 1) / 2.0 - 1)),
					imwarpdatatype(x + ((imageWidth + 1) / 2.0) - 1));
				target[k].at<imwarpdatatype>(v, u) = I2;
			}
		}
	}
	return target;
}
#endif
