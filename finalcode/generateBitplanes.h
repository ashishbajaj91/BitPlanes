#ifndef GENERATEBITPLANES_IS_INCLUDED
#define GENERATEBITPLANES_IS_INCLUDED

std::vector<cv::Mat> generateBitPlanes(cv::Mat &grayimage)
{
	cv::Mat grayImageWithBorder;
	cv::copyMakeBorder(grayimage, grayImageWithBorder, 1, 1, 1, 1, cv::BORDER_REPLICATE);

	//Create 8 channels
	std::vector<cv::Mat> image_channels;
	for (int i = 0; i < 8; i++)
	{
		cv::Mat bitplanesImage(grayimage);
		image_channels.push_back(bitplanesImage);
	}

	for (int i = 1; i<grayImageWithBorder.rows - 1; i++) 
	{
		uchar* orgp = grayImageWithBorder.ptr<uchar>(i);

		for (int j = 1; j<grayImageWithBorder.cols - 1; j++)
		{
			uchar test = orgp[j];
			uchar* newp0 = image_channels[0].ptr<uchar>(i - 1);
			uchar* newp1 = image_channels[1].ptr<uchar>(i - 1);
			uchar* newp2 = image_channels[2].ptr<uchar>(i - 1);
			uchar* newp3 = image_channels[3].ptr<uchar>(i - 1);
			uchar* newp4 = image_channels[4].ptr<uchar>(i - 1);
			uchar* newp5 = image_channels[5].ptr<uchar>(i - 1);
			uchar* newp6 = image_channels[6].ptr<uchar>(i - 1);
			uchar* newp7 = image_channels[7].ptr<uchar>(i - 1);

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