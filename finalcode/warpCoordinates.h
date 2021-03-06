#ifndef WARPCOORDINATES_IS_INCLUDED
#define WARPCOORDINATES_IS_INCLUDED

#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <cmath>

/**
 * Apply inverse warping to the rectangle
 * @input inCoords: [x1, y1, x2, y2, x3, y3, x4, y4]
 * @input image_size: [rows, cols]
 * @output warpedCoords: [x1, y1, x2, y2, x3, y3, x4, y4]
 */
void warpCoords(double warpedCoords[8], double inCoords[8], Eigen::Matrix3d warpMat, double imgWidth, double imgHeight, bool IsInverse = true)
{
    double xOffset = (imgWidth + 1)/2.0 - 1;
    double yOffset = (imgHeight + 1)/2.0 - 1;

    Eigen::MatrixXd inMatrix(3,4); // [y1, y2, y3, y4; x1, x2, x3, x4; 1, 1, 1, 1]

    for (int index = 0; index<4; index++)
    {
        inMatrix.col(index) << inCoords[index * 2 + 1] - yOffset, inCoords[index * 2] - xOffset, 1.0; // (y, x, 1)
    }
	
	if(IsInverse)
		warpMat = warpMat.inverse().eval();
    warpMat /= warpMat(2,2);

    auto newCoords = warpMat * inMatrix;

    for (int index = 0; index<4; index++)
    {
        warpedCoords[2 * index]     =   newCoords(1,index) / newCoords(2,index) + xOffset; // x
        warpedCoords[2 * index+1]   =   newCoords(0,index) / newCoords(2,index) + yOffset; // y
    }

}

/**
 * Draw bounding box on image
 * @input ptsCoords: [x1, y1, x2, y2, x3, y3, x4, y4]. Points are from left top, clockwise
 * such that [top left, top right, bottom right, bottom left] order
 * @input img2Draw: opencv image in BGR
 */
cv::Mat drawBoundingBox(double ptsCoords[8], cv::Mat &image)
{
	cv::Mat img2Draw;
	image.copyTo(img2Draw);
	cv::Scalar RED(0,0,255);
    int linethickness = 2;
    int lineType = 8;
    int shift = 0;

    for (int lineNum = 0; lineNum < 4; lineNum++)
    {
        int xi  = (int)std::round(ptsCoords[(2 * lineNum)   % 8]);
        int yi  = (int)std::round(ptsCoords[(2 * lineNum+1) % 8]);
        int xi1 = (int)std::round(ptsCoords[(2 * lineNum+2) % 8]);
        int yi1 = (int)std::round(ptsCoords[(2 * lineNum+3) % 8]);
        cv::Point pt1(xi, yi); //(xi, yi)
        cv::Point pt2(xi1, yi1); //(xi+1, yi+1)
        cv::line(img2Draw, pt1, pt2, RED, linethickness, lineType, shift);
    }

	return img2Draw;
}

#endif //INVERSEWARPCOORDS_INVERSEWARP_H_H
