#ifndef PADIMAGES_IS_INCLUDED
#define PADIMAGES_IS_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat AddPaddingToImage(cv::Mat &image, int top, int bottom, int left, int right, int val)
{
	cv::Mat result;
	cv::copyMakeBorder(image, result, top, bottom, left, right, cv::BORDER_CONSTANT, val);
	return result;
}

cv::Mat AddPaddingToImage(cv::Mat &image, int top, int bottom, int left, int right)
{
	cv::Mat result;
	cv::copyMakeBorder(image, result, top, bottom, left, right, cv::BORDER_REPLICATE);
	return result;
}

#endif