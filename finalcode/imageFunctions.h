#ifndef IMAGEFUNCTIONS_IS_INCLUDED
#define IMAGEFUNCTIONS_IS_INCLUDED

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void ApplyGaussianFilterOnPlanes(std::vector<cv::Mat> &bitPlanes, double sigma)
{
	for (int i = 0; i < bitPlanes.size(); i++)
	{
		cv::Mat result;
		cv::GaussianBlur(bitPlanes[i], result, { 0,0 }, sigma, sigma);
		bitPlanes[i] = result;
	}
}

#endif