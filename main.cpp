#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/core/mat.hpp>
#include "GridExtractor.h"
#include "GridDrawer.h"
#include "TicTacToeActivity.h"

int main() {
	cv::VideoCapture cap(0);

	TicTacToeActivity activity;

	activity.playUsing(cap);

	return 0;
}
