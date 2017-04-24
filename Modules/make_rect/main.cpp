#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>



int main( int argc, char** argv )
{
    if( argc != 2)
    {
        std::cout <<" Usage: display_image ImageToLoadAndDisplay" << std::endl;
        return -1;
    }
    ////////// LOADING AN IMAGE /////////////////
    cv::Mat image;
    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    //////////// CREATING A RECTANGLE OVER THE IMAGE ///////////
    int thickness = 1;
    int lineType = 8;

    cv::rectangle(image , cv::Point(5,10), cv::Point(20,30), cv::Scalar(255,0,0),thickness,lineType);
    
    // image is the cv::MAT object containing the image data
    // Point are the diagnolly opposite points
    // Scalar thing is the color of the line
    // Thickness and lineType are optional

    ///////////// DDIAPLAYING THE IMAGE ///////////////////////

    cv::namedWindow( "Display window");//, WINDOW_AUTOSIZE );// Create a window for display.
    cv::imshow( "Display window", image );                   // Show our image inside it.

    cv::waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}