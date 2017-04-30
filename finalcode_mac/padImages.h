#ifndef PADIMAGES_IS_INCLUDED
#define PADIMAGES_IS_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "imwarp.h"

cv::Mat AddPaddingToImage(cv::Mat &image, int top, int bottom, int left, int right, double val)
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

void AddPaddingtoBitPlaneswithNaN(std::vector<cv::Mat> &bitPlanes, int top, int bottom, int left, int right)
{
	for (int i = 0; i < bitPlanes.size(); ++i)
	{
		bitPlanes[i] = AddPaddingToImage(bitPlanes[i], top, bottom, left, right, fNaN);
	}
}

#endif