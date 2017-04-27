#ifndef READVIDEO_IS_INCLUDED
#define READVIDEO_IS_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "readImage.h"

bool readVideoFile(cv::VideoCapture &capVideo, const cv::String &filename)
{
	return (capVideo.open(filename) && capVideo.isOpened() && capVideo.get(CV_CAP_PROP_FRAME_COUNT) >= 1) ;
}

unsigned int getTotalFrames(cv::VideoCapture &capVideo)
{
	return (unsigned int)capVideo.get(CV_CAP_PROP_FRAME_COUNT);
}

unsigned int getFrameNumber(cv::VideoCapture &capVideo)
{
	return (unsigned int)capVideo.get(CV_CAP_PROP_POS_FRAMES);
}

bool IsThereFramesToRead(cv::VideoCapture &capVideo)
{
	return (getFrameNumber(capVideo) + 1 < getTotalFrames(capVideo));
}

bool readImageFrameFromVideo(cv::Mat &imageFrame, cv::VideoCapture &capVideo)
{
	return (IsThereFramesToRead(capVideo) && capVideo.read(imageFrame));
}

bool ReadAndDisplayVideo(cv::VideoCapture &capVideo, const cv::String &filename)
{
	bool readfile = readVideoFile(capVideo, filename);
	cv::String video_read_test_windowname = "Video_Read_Test";
	createNamedWindow(video_read_test_windowname);
	while (1)
	{
		cv::Mat imageFrame;
		if (!readImageFrameFromVideo(imageFrame, capVideo))
			break;
		showImage(imageFrame, video_read_test_windowname);
		char chCheckForEscKey = cv::waitKey(0);
		if (chCheckForEscKey == 27)
		{
			break;
		}
	}
	return readfile;
}

#endif