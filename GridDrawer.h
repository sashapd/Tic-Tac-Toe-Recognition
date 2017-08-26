//
// Created by Oleksandr Padko on 8/21/17.
//

#ifndef TICTACTOERECOGNITION_GRIDDRAWER_H
#define TICTACTOERECOGNITION_GRIDDRAWER_H


#include "Grid.h"

class GridDrawer
{
public:
	GridDrawer(Grid grid);

	void drawCircle(int x, int y);

	void drawCross(int x, int y);
	void drawWinnerLine(GameState game_state);

	void drawWinnerLine(int x1, int y1, int x2, int y2, Cell winner);
	void drawCells(GameState game_state);

	void drawGameState(GameState game_state);

	static const cv::Scalar kCrossColor;
	static const cv::Scalar kCircleColor;
	static const cv::Scalar kWinningLineColor;

private:
	Grid mGrid;

	void emptyCell(cv::Mat cell);
};


#endif //TICTACTOERECOGNITION_GRIDDRAWER_H
