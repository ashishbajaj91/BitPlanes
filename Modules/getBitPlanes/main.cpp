#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

cv::Mat getBitPlane(cv::Mat image)
{
    // getting the gray scale image
    Mat gimg;
    cvtColor(image , gimg, CV_BGR2GRAY);
    // creating 1 pixel border
    Mat bimg;
    copyMakeBorder( gimg, bimg, 1, 1, 1, 1, BORDER_REPLICATE);
    // creating the bit image
    Mat bitimg(gimg);
    for (int i = 1; i<bimg.rows-1;i++){
        uchar* orgp = bimg.ptr<uchar>(i);
        uchar* newp = bitimg.ptr<uchar>(i-1);
        for (int j = 1;j<bimg.cols-1;j++)
        {
            uchar test = orgp[j];
            newp[j-1] = (test > (orgp -1)[j-1]) + 2*(test > (orgp -1)[j])
                        + 4*(test > (orgp -1)[j+1]) + 8*(test > orgp[j-1])
                        + 16*(test > orgp [j+1]) + 32*(test > (orgp+1)[j-1]) 
                        + 64*(test > (orgp +1)[j]) + 128*(test > (orgp +1)[j+1]);
        }
    }
    return bitimg;
}

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: main image_name" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", getBitPlane(image) );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
