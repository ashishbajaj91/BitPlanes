#ifndef GENERATEBITPLANES_IS_INCLUDED
#define GENERATEBITPLANES_IS_INCLUDED

#include "padImages.h"

std::vector<cv::Mat> InitializeChannels(cv::Mat &grayimage)
{
	std::vector<cv::Mat> image_channels;
	for (int i = 0; i < 8; i++)
	{
		cv::Mat bitplanesImage = cv::Mat(grayimage.rows, grayimage.cols, grayimage.type(), 0.0);
		image_channels.push_back(bitplanesImage);
	}
	return image_channels;
}

std::vector<cv::Mat> generateBitPlanes(cv::Mat &grayimage)
{
	cv::Mat grayImageWithBorder = AddPaddingToImage(grayimage, 1, 1, 1, 1);
	std::vector<cv::Mat> image_channels = InitializeChannels(grayimage);

	for (int i = 1; i<grayImageWithBorder.rows - 1; i++) 
	{
		for (int j = 1; j<grayImageWithBorder.cols - 1; j++)
		{
			image_channels[0].at<double>(i-1,j-1) = grayImageWithBorder.at<double>(i,j) > grayImageWithBorder.at<double>(i-1,j-1);
			image_channels[1].at<double>(i-1,j-1) = grayImageWithBorder.at<double>(i,j) > grayImageWithBorder.at<double>(i-1,j);
			image_channels[2].at<double>(i-1,j-1) = grayImageWithBorder.at<double>(i,j) > grayImageWithBorder.at<double>(i-1,j+1);
			image_channels[3].at<double>(i-1,j-1) = grayImageWithBorder.at<double>(i,j) > grayImageWithBorder.at<double>(i,j-1);
			image_channels[4].at<double>(i-1,j-1) = grayImageWithBorder.at<double>(i,j) > grayImageWithBorder.at<double>(i,j+1);
			image_channels[5].at<double>(i-1,j-1) = grayImageWithBorder.at<double>(i,j) > grayImageWithBorder.at<double>(i+1,j-1);
			image_channels[6].at<double>(i-1,j-1) = grayImageWithBorder.at<double>(i,j) > grayImageWithBorder.at<double>(i+1,j);
			image_channels[7].at<double>(i-1,j-1) = grayImageWithBorder.at<double>(i,j) > grayImageWithBorder.at<double>(i+1,j+1);
		}
	}
	return image_channels;
}
#endif