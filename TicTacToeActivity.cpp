#include <iostream>
#include <opencv/cv.hpp>
#include "GridDrawer.h"
#include "GridExtractor.h"
#include "GameAutomate.h"
#include "TicTacToeActivity.h"
#include "TimingLogger.h"

void TicTacToeActivity::playUsing(cv::Mat img)
{
	play(img);
	cv::waitKey(0);
}

void TicTacToeActivity::playUsing(cv::VideoCapture& video_capture)
{
	if (!video_capture.isOpened())
	{
		std::cout << "Video caputre is not opened" << std::endl;
		return;
	}
	while (cv::waitKey(1) != 27)
	{
		cv::Mat frame;
		video_capture >> frame;
		video_capture >> frame;
		video_capture >> frame;
		if (!frame.empty())
		{
			play(frame);
		}
	}
}

void TicTacToeActivity::play(cv::Mat img)
{
	cv::Mat image = img;
	resize(img, image, cv::Size(1000, 1000 * img.rows / img.cols));
	GridExtractor extractor(image);
	extractor.extractGrid();
	TimingLogger::getInstance()->enableLogger();

	if (!extractor.hasFoundGrid())
	{
		std::cout << "Grid not found" << std::endl;
		imshow("image", image);
	}
	else
	{
		TimingLogger::getInstance()->onTaskStart("Frame decoding");
		Grid grid = extractor.getGrid();
		GridDrawer drawer(grid);
		GameAutomate game_automate(X);
		drawer.drawGameState(game_automate.makeTurn(grid.getGameState()));
		TimingLogger::getInstance()->onTaskEnd("Frame decoding");
		extractor.putBackGrid(grid);
		cv::Mat im = extractor.getImage();
		resize(im, im, cv::Size(1000, 1000 * im.rows / im.cols));
		imshow("image", im);
	}
}
