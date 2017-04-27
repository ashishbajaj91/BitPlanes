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
		double* orgp = grayImageWithBorder.ptr<double>(i);
		double* newp0 = image_channels[0].ptr<double>(i - 1);
		double* newp1 = image_channels[1].ptr<double>(i - 1);
		double* newp2 = image_channels[2].ptr<double>(i - 1);
		double* newp3 = image_channels[3].ptr<double>(i - 1);
		double* newp4 = image_channels[4].ptr<double>(i - 1);
		double* newp5 = image_channels[5].ptr<double>(i - 1);
		double* newp6 = image_channels[6].ptr<double>(i - 1);
		double* newp7 = image_channels[7].ptr<double>(i - 1);

		for (int j = 1; j<grayImageWithBorder.cols - 1; j++)
		{
			double test = orgp[j];

			newp0[j - 1] = (test > (orgp - 1)[j - 1]);
			newp1[j - 1] = (test > (orgp - 1)[j]);
			newp2[j - 1] = (test > (orgp - 1)[j + 1]);
			newp3[j - 1] = (test > orgp[j - 1]);
			newp4[j - 1] = (test > orgp[j + 1]);
			newp5[j - 1] = (test > (orgp + 1)[j - 1]);
			newp6[j - 1] = (test > (orgp + 1)[j]);
			newp7[j - 1] = (test > (orgp + 1)[j + 1]);
		}
	}
	return image_channels;
}
#endif