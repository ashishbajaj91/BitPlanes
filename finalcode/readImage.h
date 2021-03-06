#ifndef READIMAGE_IS_INCLUDED
#define READIMAGE_IS_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void createNamedWindow(const cv::String &windowname)
{
	cv::namedWindow(windowname, cv::WINDOW_AUTOSIZE);// Create a window for display.
}

cv::Mat convertToGrayScale(cv::Mat &image)
{
	cv::Mat gray_image;
	if (image.type() != CV_8UC1)
	{
		cv::cvtColor(image, gray_image, CV_BGR2GRAY);
		return gray_image;
	}
	else
		return image;
}

cv::Mat convertToDouble(cv::Mat &image, bool isbitplaneImage = false)
{
	cv::Mat_<double> double_image;
	image.convertTo(double_image, CV_64F);

	if (!isbitplaneImage)
		double_image /= 255.0;

	return double_image;
}

void destroyWindow(const cv::String &windowname)
{
	if (windowname.compare("All") != 0)
		cv::destroyWindow(windowname);
	else
		cv::destroyAllWindows();
}

bool readImage(cv::Mat &image, const cv::String &filename, int flag = CV_LOAD_IMAGE_COLOR)
{
	image = cv::imread(filename, flag);   // Read the file
	if (image.data)
		return true;
	return false;
}

void showImage(cv::Mat &image, const cv::String &windowname)
{
	cv::imshow(windowname, image);
}

bool ReadandShowImage(cv::Mat &image, const cv::String &filename, int flag = CV_LOAD_IMAGE_COLOR, const cv::String &windowname = "Temp")
{
	bool result = readImage(image, filename, flag);
	if(result)
	{
		createNamedWindow(windowname);
		showImage(image, windowname);
	}
	return result;
}

#endif