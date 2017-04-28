// Vaibhav Jain

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(void) {
//int main( int argc, char** argv ) {
	cv::VideoCapture capVideo;

	cv::Mat imgFrame;

	capVideo.open("768x576.avi");
	//capVideo.open(argv[1]);

	if (!capVideo.isOpened()) {                      // if unable to open video file
		printf("\nerror: reading video file\n");      // show error message
		_getch();                    // it may be necessary to change or remove this line if not using Windows
		return(0);                                                              // and exit program
	}

	if (capVideo.get(CV_CAP_PROP_FRAME_COUNT) < 1) {
		printf("\nerror: video file must have at least one frame\n");
		_getch();
		return(0);
	}

	capVideo.read(imgFrame);

	char chCheckForEscKey = 0;

	while (capVideo.isOpened() && chCheckForEscKey != 27) {

		cv::imshow("imgFrame", imgFrame);
		
		//  next iterations

		if ((capVideo.get(CV_CAP_PROP_POS_FRAMES) + 1) < capVideo.get(CV_CAP_PROP_FRAME_COUNT)) {// if there is at least one more frame
			std::cout << capVideo.get(CV_CAP_PROP_POS_FRAMES) << std::endl;
			if (capVideo.get(CV_CAP_PROP_POS_FRAMES) == 99) { // to stop at a particular frame
				chCheckForEscKey = cv::waitKey(0);
			}

			capVideo.read(imgFrame);
			/*chCheckForEscKey = cv::waitKey(0);  // uncomment only when you need to move frame by frame using space key
			if (chCheckForEscKey == 27) {		  // while moving frame by frame, if you need to close the window and exit
				break;
			}*/
		}
		else {                                                  // else
			std::cout << "end of video\n";                      // show end of video message
			break;                                              // and jump out of while loop
		}

		chCheckForEscKey = cv::waitKey(1);      // get key press in case user pressed esc

	}

	if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
		cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
	}
	// note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows

	return(0);
}
