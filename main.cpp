#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/core/mat.hpp>
#include "GridExtractor.h"
#include "GridDrawer.h"
#include "TicTacToeActivity.h"

int main() {
	cv::Mat img = cv::imread("board1.jpg");

    cv::VideoCapture videoCapture;
    videoCapture.open(0);

	TicTacToeActivity activity;

	activity.playUsing(videoCapture);

	return 0;
}
