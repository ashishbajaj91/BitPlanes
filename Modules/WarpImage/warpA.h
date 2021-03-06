#ifndef WARPIMAGE_WARPA_H
#define WARPIMAGE_WARPA_H

#include <iostream>
#include <Eigen/Dense>
#include<opencv2/opencv.hpp>
#include <cmath>

double fNaN = std::numeric_limits<double>::quiet_NaN();

template <class T>
T Interpolate(cv::Mat &image, double y, double x)
{
    double xd, yd;
    int xi = int(floor(x));
    int yi = int(floor(y));
    double k1 = modf(x, &xd); // k1 is the fractional, xd is the integer of x
    double k2 = modf(y, &yd);

    // std::cout << "image rows " << image.rows << " image cols " << image.cols << std::endl;
    // Check whether the pixels are within the image
    if (xi < 0 || xi > image.cols - 1 || yi < 0 || yi > image.rows - 1)
    {
        return fNaN;
    }
    //if(xi >= 0 && (xi <= (int)image.cols-1) && yi >=0 && (yi <= (int)image.rows-1))
    //if(x >= 0.0 && (x <= (1.0*image.cols-1)) && y >=0 && (y <= (1.0*image.rows-1)))
    else
    {
        int f1 = xi <= image.cols - 2; // Check that pixels to the right
        int f2 = yi <= image.rows - 2; // and to down direction exist.
        T px1 = image.at<T>(yi, xi);
        T px2 = image.at<T>(yi, xi + 1);
        T px3 = image.at<T>(yi + 1, xi);
        T px4 = image.at<T>(yi + 1, xi + 1);
        T interpolated_value = (1.0 - k1) * (1.0 - k2) * px1 +
                               (f1 ? (k1 * (1.0 - k2) * px2) : 0) +
                               (f2 ? ((1.0 - k1) * k2 * px3) : 0) +
                               ((f1 && f2) ? (k1 * k2 * px4) : 0);
        return interpolated_value;
    }

//    else
//    {
//        return fNaN;
//    }
}

/**
 * imgSrc: input image
 * warpMat: warping function, 3x3
 * out_size: output image size, [rows, cols]
 */
cv::Mat ApplyWarp(cv::Mat imgSrc, Eigen::Matrix3d warpMat, int out_size[])
{
    int imageWidth = imgSrc.cols;
    int imageHeight = imgSrc.rows;

    // Set target image size and type
    int targetRows = out_size[0];
    int targetCols = out_size[1];
    cv::Mat target = cv::Mat(targetRows, targetCols, imgSrc.type(), cv::Scalar(fNaN));

    warpMat = warpMat.inverse().eval();
    warpMat /= warpMat(2,2);

    Eigen::Vector3d X;      // Point in coordinate frame of source.
    Eigen::Vector3d U;      // Point in coordinate frame of target.

    for(int v = 0; v < targetRows; v++)
    {
        for (int u = 0; u < targetCols; u++)
        {
            U = Eigen::Vector3d(v - ((imageHeight + 1) / 2.0 - 1), u - ((imageWidth + 1) / 2.0 - 1), 1.0);
            X = warpMat * U;
            double y = X(0) / X(2);
            double x = X(1) / X(2);
            y = double(y + ((imageHeight + 1) / 2.0 - 1));
            x = double(x + ((imageWidth + 1) / 2.0) - 1);
            double I2 = Interpolate<double>(imgSrc, y,
                                          x);
            target.at<double>(v, u) = I2;
            std::cout << y << " " << x << " " << v << "," << u << " " <<I2 << std::endl;
        }
    }
    return target;
}

#endif //WARPIMAGE_WARPA_H
