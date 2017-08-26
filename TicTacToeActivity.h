#ifndef TICTACTOERECOGNITION_TICTACTOE_H
#define TICTACTOERECOGNITION_TICTACTOE_H

#include <opencv2/videoio.hpp>
#include <opencv2/core/core.hpp>


class TicTacToeActivity
{
public:
	void playUsing(cv::Mat img);

	void playUsing(cv::VideoCapture& video_capture);

private:
	void play(cv::Mat img);
};

#endif // TICTACTOERECOGNITION_TICTACTOE_H
