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

	void drawWinnerLine(int x1, int y1, int x2, int y2, Cell winner);

	void drawGameState(GameState game_state);

private:
	Grid mGrid;
	const cv::Scalar crossColor;
	const cv::Scalar circleColor;

	void emptyCell(cv::Mat cell);
};


#endif //TICTACTOERECOGNITION_GRIDDRAWER_H
