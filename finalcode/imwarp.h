#ifndef WARPIMAGE_WARPA_H
#define WARPIMAGE_WARPA_H

#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <cmath>

typedef float imwarpdatatype;

imwarpdatatype fNaN = std::numeric_limits<imwarpdatatype>::quiet_NaN();

template <class T>
T Interpolate(cv::Mat &image, imwarpdatatype  y, imwarpdatatype  x)
{
	imwarpdatatype xd, yd;

	int xi = int(floor(x));
	int yi = int(floor(y));
	
	imwarpdatatype k1 = modff(x, &xd); // k1 is the fractional, xd is the integer of x
	imwarpdatatype k2 = modff(y, &yd);

	// Check whether the pixels are within the image
	if (xi >= 0 && (xi < image.cols - 1) && yi >= 0 && (yi < image.rows - 1))
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

/**
* imgSrc: input image
* warpMat: warping function, 3x3
* out_size: output image size, [rows, cols]
*/
cv::Mat ApplyWarp(cv::Mat imgSrc, Eigen::Matrix3f warpMat, int out_size[])
{
	int imageWidth = imgSrc.cols;
	int imageHeight = imgSrc.rows;

	// Set target image size and type
	int targetRows = out_size[0];
	int targetCols = out_size[1];

	cv::Mat target = InitializeTargetImage(out_size[0], out_size[1], imgSrc.type(), cv::Scalar(fNaN));
	warpMat = warpMat.inverse().eval();
	warpMat /= warpMat(2, 2);

	Eigen::Vector3f X;      // Point in coordinate frame of source.
	Eigen::Vector3f U;      // Point in coordinate frame of target.

	for (int v = 0; v < targetRows; v++)
	{
		for (int u = 0; u < targetCols; u++)
		{
			U = Eigen::Vector3f(v - ((imageHeight + 1) / 2.0 - 1), u - ((imageWidth + 1) / 2.0 - 1), 1.0);
			X = warpMat * U;
			imwarpdatatype  y = X(0) / X(2);
			imwarpdatatype  x = X(1) / X(2);
			imwarpdatatype  I2 = Interpolate<imwarpdatatype >(imgSrc, imwarpdatatype(y + ((imageHeight + 1) / 2.0 - 1)),
				imwarpdatatype(x + ((imageWidth + 1) / 2.0) - 1));
			target.at<float>(v, u) = I2;
		}
	}
	return target;
}

#endif