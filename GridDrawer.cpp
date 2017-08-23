//
// Created by Oleksandr Padko on 8/21/17.
//

#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv/cv.hpp>
#include "GridDrawer.h"

GridDrawer::GridDrawer(Grid grid) : crossColor(255, 0, 0), circleColor(0, 0, 255) {
    mGrid = grid;
}

void GridDrawer::drawCircle(int x, int y) {
    cv::Mat cellImg = mGrid.getCellImage(x, y);
    emptyCell(cellImg);
    int radius = cellImg.cols * 0.35;
    cv::Point center(cellImg.cols / 2, cellImg.rows / 2);
    cv::circle(cellImg, center, radius, circleColor, 3);
}

void GridDrawer::drawCross(int x, int y) {
    cv::Mat cellImg = mGrid.getCellImage(x, y);
    emptyCell(cellImg);
    int offset = cellImg.cols * 0.2;
    cv::Point p1(offset, offset), p2(cellImg.cols - offset, cellImg.rows - offset);
    cv::Point p3(offset, cellImg.rows - offset), p4(cellImg.cols - offset, offset);
    cv::line(cellImg, p1, p2, crossColor, 3);
    cv::line(cellImg, p3, p4, crossColor, 3);
}

void GridDrawer::drawWinnerLine(GameState game_state) {
	if (!game_state.isOsWin() && !game_state.isXsWin())
	{
		return;
	}
	std::vector<int> line_points = game_state.isOsWin() 
		? game_state.getCompleteLineFor(O) : game_state.getCompleteLineFor(X);
	cv::Scalar color = game_state.isOsWin() ? circleColor : crossColor;
    cv::Mat gridImage = mGrid.getImage();
    int cellWidth = gridImage.cols / 3;
    int cellHeigh = gridImage.rows / 3;
	cv::Point p1(line_points[0] * cellWidth + cellWidth / 2, line_points[1] * cellHeigh + cellHeigh / 2);
    cv::Point p2(line_points[2] * cellWidth + cellWidth / 2, line_points[3] * cellHeigh + cellHeigh / 2);
    cv::line(gridImage, p1, p2, color, 3);
}

void GridDrawer::drawCells(GameState game_state)
{
	for (int row = 0; row < GameState::kRowCount; ++row)
	{
		for (int column = 0; column < GameState::kColumnsCount; ++column)
		{
			auto cell = game_state.getCell(row, column);
			if (cell == O)
			{
				drawCircle(row, column);
			} else if (cell == X)
			{
				drawCross(row, column);
			}
		}
	}
}

void GridDrawer::drawGameState(GameState game_state)
{
	drawCells(game_state);
	drawWinnerLine(game_state);
}

void GridDrawer::emptyCell(cv::Mat cellImg) {
    int offset = cellImg.cols / 15;
    cv::Rect roi(offset, offset, cellImg.cols - offset * 2, cellImg.rows - offset * 2);
    cv::Mat cellImgRoi = cellImg(roi);
    int morphSize = 5;
    cv::Mat morphElement = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                     cv::Size(2 * morphSize + 1, 2 * morphSize + 1),
                                                     cv::Point(morphSize, morphSize));
    cv::Mat eroded;
    cv::dilate(cellImgRoi, eroded, morphElement);
    eroded.copyTo(cellImgRoi);
}
