#include <iostream>
#include <Eigen/Dense>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace Eigen;
using namespace cv;

double dNaN = std::numeric_limits<double>::quiet_NaN();

template <class T>
T Interpolate(cv::Mat &image, float y, float x)
{
    float xd, yd;
    float k1 = modff(x, &xd); // k1 is the fractional, xd is the integer of x
    float k2 = modff(y, &yd);
    int xi = int(xd);
    int yi = int(yd);
    // Check whether the pixels are within the image
    if(xi >= 0 && xi < image.cols && yi >=0 && yi < image.rows)
    {
        int f1 = xi < image.rows - 1; // Check that pixels to the right
        int f2 = yi < image.cols - 1; // and to down direction exist.
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
    else
    {
        return dNaN; // if pixel is not within the image
    }
}

int main(int argc, char** argv) {
    namedWindow("original image");
    namedWindow("warped image");

    string filename;
    if (argc > 2)
    {
        filename = argv[1];
    }
    else filename = "/Users/shunyao/cv_imgs/lena_color.jpeg";

    // Load image
    Mat img = imread(filename,0);;
    cv::Mat img_src_f; img.convertTo(img_src_f, CV_32FC1);
    img_src_f /= 255; // rescale to float

    // Set target image size and type
    int targetRows = 225;
    int targetCols = 225;
    cv::Mat target = Mat(targetRows, targetCols, img_src_f.type(), cv::Scalar(dNaN));

    Matrix3f W(3,3); // warp function
    W <<    1-0.707,  0.707, 0,
            0.707,  1-0.707,  0,
            0,  0,  1;
    W = W.inverse().eval();
    cout << W << endl;
    Vector3f X;      // Point in coordinate frame of source.
    Vector3f U;      // Point in coordinate frame of target.

    for(int v = 0; v < target.rows; v++)
    {
        for (int u = 0; u < target.cols; u++)
        {
            U = Vector3f(u, v, 1.0);
            X = W * U; // get coordinate in source image, X

            float x = X(0);
            float y = X(1);
            {
                float I2 = Interpolate<float>(img_src_f, y, x);
                target.at<float>(v, u) = I2;
            }
        }
    }

    imshow("original image", img_src_f);
    imshow("warped image", target);

    int k = cv::waitKey(0);

}