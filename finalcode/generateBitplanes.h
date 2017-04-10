#ifndef GENERATEBITPLANES_IS_INCLUDED
#define GENERATEBITPLANES_IS_INCLUDED

cv::Mat generateBitPlanes(cv::Mat &grayimage)
{
	cv::Mat grayImageWithBorder;
	cv::copyMakeBorder(grayimage, grayImageWithBorder, 1, 1, 1, 1, cv::BORDER_REPLICATE);

	cv::Mat bitplanesImage(grayimage);

	for (int i = 1; i<grayImageWithBorder.rows - 1; i++) 
	{
		uchar* orgp = grayImageWithBorder.ptr<uchar>(i);
		uchar* newp = bitplanesImage.ptr<uchar>(i - 1);

		for (int j = 1; j<grayImageWithBorder.cols - 1; j++)
		{
			uchar test = orgp[j];
			newp[j - 1] = (test >(orgp - 1)[j - 1]) + 2 * (test >(orgp - 1)[j])
				+ 4 * (test > (orgp - 1)[j + 1]) + 8 * (test > orgp[j - 1])
				+ 16 * (test > orgp[j + 1]) + 32 * (test > (orgp + 1)[j - 1])
				+ 64 * (test > (orgp + 1)[j]) + 128 * (test > (orgp + 1)[j + 1]);
		}
	}
	return bitplanesImage;
}

#endif