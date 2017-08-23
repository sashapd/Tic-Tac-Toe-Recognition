#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/core/mat.hpp>
#include "GridExtractor.h"
#include "GridDrawer.h"
#include "TicTacToeActivity.h"

int main() {
	cv::Mat img = cv::imread("sample2.png");

	TicTacToeActivity activity;

	cv::VideoCapture video_capture;
	video_capture.open(0);
	activity.playUsing(img);

	return 0;
}
