#ifndef MATRIXFUNCTIONS_IS_INCLUDED
#define MATRIXFUNCTIONS_IS_INCLUDED

#include <Eigen/Dense>

cv::Mat CheckForNaN(cv::Mat &image)
{
	cv::Mat IsNaN = cv::Mat_<double>(image != image)/255;
	return IsNaN;
}

cv::Mat CheckForNotNaN(cv::Mat &image)
{
	cv::Mat IsNotNaN = cv::Mat_<double>(image == image) / 255;
	return IsNotNaN;
}

cv::Mat CheckForNaNinPlanes(std::vector<cv::Mat> &image)
{
	cv::Mat IsNaN;
	for (int i = 0; i < image.size(); i++)
	{
		if (i == 0)
			IsNaN = CheckForNaN(image[i]);
		else
		{
			IsNaN = cv::Mat_<double>(IsNaN | CheckForNaN(image[i]));
		}
	}
	return IsNaN;
}

cv::Mat CheckForNotNaNinPlanes(std::vector<cv::Mat> &image)
{
	cv::Mat IsNotNaN;
	for (int i = 0; i < image.size(); i++)
	{
		if (i == 0)
			IsNotNaN = CheckForNotNaN(image[i]);
		else
		{
			IsNotNaN = cv::Mat_<double>(IsNotNaN & CheckForNotNaN(image[i]));
		}
	}
	return IsNotNaN;
}

#endif