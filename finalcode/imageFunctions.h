#ifndef IMAGEFUNCTIONS_IS_INCLUDED
#define IMAGEFUNCTIONS_IS_INCLUDED

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat ApplyGaussianFilter(cv::Mat &image, double sigma)
{
	cv::Mat result;
	cv::GaussianBlur(image, result, { 0, 0 }, sigma, sigma);
	return result;
}

void ApplyGaussianFilterOnPlanes(std::vector<cv::Mat> &bitPlanes, double sigma)
{
	for (int i = 0; i < bitPlanes.size(); i++)
	{
		bitPlanes[i] = ApplyGaussianFilter(bitPlanes[i], sigma);
	}
}
#endif