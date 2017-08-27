#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"  
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
String window_name = "Motion detector";
String window_mask = "Mask";


int main(void)
{
	Mat frame;
	Mat mask, nonzerolocations;
	VideoCapture capture;

	Ptr<BackgroundSubtractor> bs = createBackgroundSubtractorMOG2();
	capture.open(0);
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }

	namedWindow(window_name);

	while (true)
	{
		capture >> frame;
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		(*bs).apply(frame, mask, 0.01);

		mask = mask > 150;

		morphologyEx(mask, mask, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(5, 5)));

		//-- Show what you got
		imshow(window_name, frame);
		imshow(window_mask, mask);
		findNonZero(mask, nonzerolocations);
		if (nonzerolocations.total() > 10) {
			imshow(window_mask, mask);
		}
		else {
			putText(frame, "We have almost no motion. Hurray!", cvPoint(30, 30),
				FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
			imshow(window_mask, frame);
		}
		//-- bail out if escape was pressed
		char c = (char)waitKey(10);
		if (c == 27) { break; }
	}

	return 0;
}