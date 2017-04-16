#include <iostream>
#include <Eigen/Dense>
#include<opencv2/opencv.hpp>
#include <cmath>
#include "warpA.h"


int main(int argc, char** argv) {
    cv::namedWindow("original image");
    cv::namedWindow("warped image");

    std::string filename;
    if (argc > 2)
    {
        filename = argv[1];
    }
    else filename = "/Users/shunyao/cv_imgs/lena_color.jpeg";

    // Load image
    cv::Mat img = cv::imread(filename,0);
    cv::Mat img_src_f; img.convertTo(img_src_f, CV_32FC1);
    img_src_f /= 255; // rescale to float


//    cv::Mat img_src_f = (cv::Mat_<float>(4,5) <<
//            0.3000, 0.6000, 0.9000, 0.1000, 0.5000,
//            0.1000, 0.2000, 0.3000, 0.4000, 0.5000,
//            0.3000, 0.9000, 0.8000, 0.7000, 0.6000,
//            0.2000, 0.4000, 0.6000, 0.8000, 1.0000);

    Eigen::Matrix3f W(3,3); // warp function

    //W = Matrix3f::Identity(3, 3);

    W <<    0.7,    -0.7,      0,
            0.7,      0.7,    0,
            0,          0,  1;

//    W <<    0,    1,      0,
//            1,      0,    0,
//            0,          0,  1;
//
    cv::Mat target;

    int out_size[] = {img_src_f.rows, img_src_f.cols};
    target = ApplyWarp(img_src_f, W, out_size);

    //std::cout << "target Pixels" << std::endl;
    //std::cout << target << std::endl;
    imshow("original image", img_src_f);
    imshow("warped image", target);

    int k = cv::waitKey(0);
}

