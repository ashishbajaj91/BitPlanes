#ifndef MATRIXFUNCTIONS_IS_INCLUDED
#define MATRIXFUNCTIONS_IS_INCLUDED

#include <Eigen/Dense>

void ComputeSVD(double *s, Eigen::Matrix3d H)
{
	Eigen::JacobiSVD<Eigen::Matrix3d> svd(H);
	auto sing = svd.singularValues();
	s[0] = sing[0];
	s[1] = sing[1];
	s[2] = sing[2];
}

cv::Mat CheckForNaN(cv::Mat &image)
{
	cv::Mat IsNaN = cv::Mat(image != image);
	return IsNaN;
}

cv::Mat CheckForNaNinPlanes(std::vector<cv::Mat> &image)
{
	cv::Mat IsNaN;
	for (int i = 0; i < image.size(); i++)
	{
		if (i == 0)
			IsNaN = CheckForNaN(image[i]);
		else
			IsNaN = cv::Mat(IsNaN | CheckForNaN(image[i]));
	}
	return IsNaN;
}

cv::Mat CheckForNotNaNinPlanes(std::vector<cv::Mat> &image)
{
	return cv::Mat(~CheckForNaNinPlanes(image));
}

#endif