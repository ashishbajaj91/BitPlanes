#ifndef IMAGEFUNCTIONS_IS_INCLUDED
#define IMAGEFUNCTIONS_IS_INCLUDED

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat ExtractAreaOfInterest(cv::Mat image, cv::Rect Rect)
{
	cv::Mat result = image(Rect);
	return result;
}

std::vector<cv::Mat> ExtractAreaOfInterestFromPlanes(std::vector<cv::Mat> imagePlanes, cv::Rect Rect)
{
	std::vector<cv::Mat> result;
	for (int i = 0; i < imagePlanes.size(); i++)
	{
		cv::Mat temp = ExtractAreaOfInterest(imagePlanes[i], Rect);
		result.push_back(temp);
	}
	return result;
}

cv::Mat ApplyGaussianFilter(cv::Mat image, double sigma)
{
	cv::Mat_<double> result;
	//cv::GaussianBlur(image, result, cv::Size(2*ceil(2*sigma) + 1, 2 * ceil(2 * sigma) + 1) , 0, 0, cv::BORDER_REPLICATE);
	cv::GaussianBlur(image, result, cv::Size(0,0), sigma, sigma, cv::BORDER_REPLICATE);
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