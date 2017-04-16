#ifndef WARPIMAGE_TEST_H
#define WARPIMAGE_TEST_H
#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "warpA.h"

bool testApplyWarpEqual(cv::Mat input, Eigen::Matrix3f warpFunc, cv::Mat expected, double thresh)
{
    cv::Mat target;
    int out_size[] = {input.rows, input.cols};
    Eigen::Matrix3f warpFunc(3,3);
    warpFunc <<
            0.7,    -0.7,   0,
            0.7,    0.7,    0,
            0,      0,      1;
    target = ApplyWarp(input, warpFunc, out_size);
    return (cv::norm(target, expected) < thresh);
}

void testApplyWarpImage(std::string filename, Eigen::Matrix3f warpFunc)
{
    // Load image
    cv::Mat img = cv::imread(filename,0);
    cv::Mat img_src_f;
    img.convertTo(img_src_f, CV_32FC1);
    img_src_f /= 255;
    cv::Mat output;
    int out_size[] = {img_src_f.rows, img_src_f.cols};
    output = ApplyWarp(img_src_f, warpFunc, out_size);
    cv::imshow("original image", img);
    cv::imshow("warped image", output);
}

void RunTests(int argc, char** argv)
{
    std::cout << "Starting the tests" << std::endl;
    if (argc == 2)
    {
        testApplyWarpImage(argv[0], argv[1]);
        cv::waitKey(0);
        cv::destroyWindow("All");
    }
    else if (argc == 4)
    {
        testApplyWarpEqual();
        destroyWindow("All");
    }
    else
    {
        std::cout << "Number of arguments should be 2 or 4" << std::endl;
    }
    std::cout << "All Tests Done" << std::endl;
}


#endif //WARPIMAGE_TEST_H
