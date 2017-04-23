#ifndef GRADIENT_IS_INCLUDED
#define GRADIENT_IS_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void ComputeImageGradient(cv::Mat gradientImage, cv::Mat Mref, int &K, cv::Mat &Iref, cv::Mat &Keep, cv::Mat &wts)
{

}

bool SubtractImages(cv::Mat &result, cv::Mat &image1, cv::Mat &image2)
{
	if (image1.cols != image2.cols || image1.rows != image2.rows || image1.dims != image2.dims)
		return false;
	cvAbsDiff(&image1, &image2, &result);
	return true;
}

bool SubtractBitPlanes(cv::Mat &result, cv::Mat &image1, cv::Mat &image2)
{

}
#endif